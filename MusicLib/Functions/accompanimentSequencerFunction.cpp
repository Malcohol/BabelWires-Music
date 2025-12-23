/**
 * Function which join tracks together sequencially.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/accompanimentSequencerFunction.hpp>

#include <MusicLib/Functions/appendTrackFunction.hpp>
#include <MusicLib/Functions/excerptFunction.hpp>
#include <MusicLib/Functions/repeatFunction.hpp>
#include <MusicLib/Functions/transposeFunction.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Path/path.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/TypeSystem/valuePathUtils.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Types/Record/recordValue.hpp>
#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>

namespace {
    /// This determines which chords get transposed upwards and which chords get transposed down.
    /// Value 6 means that C#..F get transposed upwards, while F#..B get transposed downwards.
    // Note: There's a similar value in fitToChordFunction.cpp.
    constexpr static int s_topChordRoot = 6;

    /// Get a track segment that matches the required duration, repeating or truncating as needed.
    bw_music::Track getTrackSegmentForDuration(const bw_music::Track& sourceTrack, bw_music::ModelDuration offset,
                                               bw_music::ModelDuration targetDuration) {
        assert(targetDuration > 0 && "Target duration must be positive");
        assert(offset >= 0 && "Offset must be non-negative");
        assert(offset < sourceTrack.getDuration() && "Offset must be within source track duration");

        const bw_music::ModelDuration sourceLength = sourceTrack.getDuration();
        if (sourceLength == 0) {
            return bw_music::Track(targetDuration);
        }

        // Even if the offset is zero, call the first excerpt a lead-in.
        const bw_music::ModelDuration leadInDuration = std::min(sourceLength - offset, targetDuration);
        auto [repeatCount, remainder] = (targetDuration - leadInDuration).divmod(sourceLength);

        // Start with the lead-in excerpt.
        bw_music::Track result = bw_music::getTrackExcerpt(sourceTrack, offset, leadInDuration);

        // Repeat full cycles
        if (repeatCount > 0) {
            bw_music::Track repeat = bw_music::repeatTrack(sourceTrack, repeatCount);
            bw_music::appendTrack(result, repeat);
        }

        // Add the remainder if needed
        if (remainder > 0) {
            bw_music::Track excerptTrack = bw_music::getTrackExcerpt(sourceTrack, 0, remainder);
            bw_music::appendTrack(result, excerptTrack);
        }

        return result;
    }

    struct AccompanimentSequencer {
        /// Accompaniment is permitted to have arbitrary structure. To avoid the complexity of traversing typed
        /// structures multiple times while processing the music data, we use this data structure.
        struct TrackInStructure {
            /// The path to the track within the accompaniment for a given chord (and also within the result value).
            babelwires::Path m_pathToTrack;
            /// The track being built.
            bw_music::Track m_track;
        };

        const babelwires::TypeSystem& m_typeSystem;
        const babelwires::RecordType& m_typeOfAccompanimentTracks;
        const babelwires::ValueHolder& m_accompanimentTracks;
        /// The type of fields within m_typeOfAccompanimentTracks.
        const babelwires::Type* m_fieldType = nullptr;
        /// The value of the first field within m_accompanimentTracks.
        babelwires::ValueHolder m_result;
        /// Map from chord type to child index in the record.
        std::map<bw_music::ChordType::Value, int> m_chordTypeToChildIndex;
        std::vector<TrackInStructure> m_tracksInStructure;
        /// Accompaniment tracks are expected to have this duration (or be empty).
        bw_music::ModelDuration m_durationOfAccompanimentTracks = 0;

        AccompanimentSequencer(const babelwires::TypeSystem& typeSystem,
                               const babelwires::RecordType& typeOfAccompanimentTracks,
                               const babelwires::ValueHolder& accompanimentTracks)
            : m_typeSystem(typeSystem)
            , m_typeOfAccompanimentTracks(typeOfAccompanimentTracks)
            , m_accompanimentTracks(accompanimentTracks) {

            // Map chord types to child indices.
            const babelwires::RecordType& recordType = typeOfAccompanimentTracks.is<babelwires::RecordType>();
            const auto& chordTypeType = typeSystem.getEntryByType<bw_music::ChordType>();
            const unsigned int numChildren = recordType.getNumChildren(accompanimentTracks);
            for (unsigned int i = 0; i < numChildren; ++i) {
                auto [fieldValue, step, fieldTypeRef] = recordType.getChild(accompanimentTracks, i);
                const int enumIndex = chordTypeType.tryGetIndexFromIdentifier(*step.asField());
                if (enumIndex >= 0) {
                    m_chordTypeToChildIndex[static_cast<bw_music::ChordType::Value>(enumIndex)] = static_cast<int>(i);
                    const babelwires::Type& fieldType = fieldTypeRef.resolve(typeSystem);
                    if (m_fieldType == nullptr) {
                        m_fieldType = &fieldType;
                        assert(!m_result);
                        m_result = *fieldValue;
                        // Use the first field to build the structure of tracks.
                        findTracksInStructure(*m_fieldType, *fieldValue);
                        if (m_durationOfAccompanimentTracks == 0) {
                            throw babelwires::ModelException()
                                << "At least one accompaniment track for each chord must have a non-zero duration";
                        }
                    } else if (m_fieldType != &fieldType) {
                        throw babelwires::ModelException()
                            << "All fields in accompanimentTracks must have the same type";
                    } else {
                        validateTracksInStructure(*m_fieldType, *fieldValue);
                    }
                }
            }
            if (!m_result) {
                throw babelwires::ModelException() << "No suitable fields found in accompanimentTracks";
            }
        }

        void addAccompanimentToTracks(const babelwires::ValueHolder& accompanimentForChord,
                                      bw_music::ModelDuration offset, bw_music::ModelDuration targetDuration,
                                      int pitchOffset) {
            for (auto& trackInStructure : m_tracksInStructure) {
                auto optFieldValue =
                    tryFollowPath(m_typeSystem, *m_fieldType, trackInStructure.m_pathToTrack, accompanimentForChord);
                if (optFieldValue) {
                    const auto& [fieldType, fieldValue] = *optFieldValue;
                    const bw_music::Track* const accompanimentTrack = fieldValue->as<bw_music::Track>();
                    if (accompanimentTrack) {
                        bw_music::Track excerpt =
                            getTrackSegmentForDuration(*accompanimentTrack, offset, targetDuration);
                        excerpt = bw_music::transposeTrack(excerpt, pitchOffset,
                                                           bw_music::TransposeOutOfRangePolicy::MapToNearestOctave);
                        // Get the track segment for this duration
                        bw_music::appendTrack(trackInStructure.m_track, excerpt);
                        continue;
                    }
                }
                bw_music::appendTrack(trackInStructure.m_track, bw_music::Track(targetDuration));
            }
        }

        void addSilenceToTracks(bw_music::ModelDuration duration) {
            for (auto& trackInStructure : m_tracksInStructure) {
                bw_music::appendTrack(trackInStructure.m_track, bw_music::Track(duration));
            }
        }

        void sequenceAccompaniment(const bw_music::Track& chordTrack) {
            std::optional<bw_music::ModelDuration> totalTimeSinceFirstChordEvent;
            bw_music::ModelDuration timeSinceLastChordEvent = 0;
            std::optional<bw_music::Chord> currentChord;

            for (const auto& event : chordTrack) {
                timeSinceLastChordEvent += event.getTimeSinceLastEvent();
                if (totalTimeSinceFirstChordEvent.has_value()) {
                    *totalTimeSinceFirstChordEvent += timeSinceLastChordEvent;
                }

                if (const auto* chordOnEvent = event.as<bw_music::ChordOnEvent>()) {
                    if (!totalTimeSinceFirstChordEvent.has_value()) {
                        totalTimeSinceFirstChordEvent = 0;
                    }
                    if (timeSinceLastChordEvent > 0) {
                        addSilenceToTracks(timeSinceLastChordEvent);
                        timeSinceLastChordEvent = 0;
                    }
                    currentChord = chordOnEvent->m_chord;
                } else if (event.as<bw_music::ChordOffEvent>()) {
                    assert(currentChord.has_value() && "ChordOffEvent without a preceding ChordOnEvent");
                    if (timeSinceLastChordEvent > 0) {
                        const auto childIndexIt = m_chordTypeToChildIndex.find(currentChord->m_chordType);
                        if (childIndexIt != m_chordTypeToChildIndex.end()) {
                            const auto& [child, _, childType] =
                                m_typeOfAccompanimentTracks.getChild(m_accompanimentTracks, childIndexIt->second);
                            const int currentRoot = static_cast<unsigned int>(currentChord->m_root);
                            const int pitchOffset = (currentRoot > s_topChordRoot) ? (currentRoot - 12) : currentRoot;

                            // Offset the time since the first chord event to align accompaniment tracks.
                            assert(totalTimeSinceFirstChordEvent.has_value());
                            auto [div, offset] = (*totalTimeSinceFirstChordEvent - timeSinceLastChordEvent)
                                                     .divmod(m_durationOfAccompanimentTracks);

                            addAccompanimentToTracks(*child, offset, timeSinceLastChordEvent, pitchOffset);
                        } else {
                            addSilenceToTracks(timeSinceLastChordEvent);
                        }
                        timeSinceLastChordEvent = 0;
                    }
                }
            }
            for (auto& trackInStructure : m_tracksInStructure) {
                trackInStructure.m_track.setDuration(chordTrack.getDuration());
            }
        }

        babelwires::TypeRef getResultTypeRef() const {
            return m_fieldType ? m_fieldType->getTypeRef() : babelwires::TypeRef();
        }

        babelwires::ValueHolder getResultValue() {
            // Build the result value by assigning tracks back into the structure.
            babelwires::ValueHolder result = m_result;
            result.copyContentsAndGetNonConst();
            assignTracksInStructure(*m_fieldType, result);
            return result;
        }

        void findTracksInStructure(const babelwires::Type& currentType, const babelwires::ValueHolder& currentValue,
                                   babelwires::Path currentPath = babelwires::Path()) {
            if (const auto* trackType = currentType.as<bw_music::TrackType>()) {
                m_tracksInStructure.push_back(TrackInStructure{currentPath});
                const bw_music::Track& sourceTrack = currentValue->is<bw_music::Track>();
                if (m_durationOfAccompanimentTracks == 0) {
                    m_durationOfAccompanimentTracks = sourceTrack.getDuration();
                } else if (sourceTrack.getDuration() != 0 &&
                           sourceTrack.getDuration() != m_durationOfAccompanimentTracks) {
                    // TODO More detail
                    throw babelwires::ModelException()
                        << "All accompaniment tracks must have the same duration (or be empty)";
                }
            } else if (const auto* recordType = currentType.as<babelwires::CompoundType>()) {
                const unsigned int numChildren = recordType->getNumChildren(currentValue);
                for (unsigned int i = 0; i < numChildren; ++i) {
                    auto [childValue, step, childTypeRef] = recordType->getChild(currentValue, i);
                    const babelwires::Type& childType = childTypeRef.resolve(m_typeSystem);
                    babelwires::Path newPath = currentPath;
                    newPath.pushStep(*step.asField());
                    findTracksInStructure(childType, *childValue, newPath);
                }
            }
        }

        void validateTracksInStructure(const babelwires::Type& currentType, const babelwires::ValueHolder& currentValue,
                                       babelwires::Path currentPath = babelwires::Path()) const {
            if (const auto* trackType = currentType.as<bw_music::TrackType>()) {
                auto it = std::find_if(
                    m_tracksInStructure.begin(), m_tracksInStructure.end(),
                    [&currentPath](const TrackInStructure& tis) { return tis.m_pathToTrack == currentPath; });
                if (it != m_tracksInStructure.end()) {
                    const bw_music::Track& sourceTrack = currentValue->is<bw_music::Track>();
                    if (sourceTrack.getDuration() != 0 &&
                        sourceTrack.getDuration() != m_durationOfAccompanimentTracks) {
                        // TODO More detail
                        throw babelwires::ModelException()
                            << "All accompaniment tracks must have the same duration (or be empty)";
                    }
                } else {
                    // Ignore tracks that are not in the structure found from the first field.
                    // They will be treated as empty in the main algorithm.
                    // TODO Warn?
                }
            } else if (const auto* recordType = currentType.as<babelwires::CompoundType>()) {
                const unsigned int numChildren = recordType->getNumChildren(currentValue);
                for (unsigned int i = 0; i < numChildren; ++i) {
                    auto [childValue, step, childTypeRef] = recordType->getChild(currentValue, i);
                    const babelwires::Type& childType = childTypeRef.resolve(m_typeSystem);
                    babelwires::Path newPath = currentPath;
                    newPath.pushStep(*step.asField());
                    validateTracksInStructure(childType, *childValue, newPath);
                }
            }
        }

        void assignTracksInStructure(const babelwires::Type& currentType, babelwires::ValueHolder& currentValue,
                                     babelwires::Path currentPath = babelwires::Path()) {
            if (const auto* trackType = currentType.as<bw_music::TrackType>()) {
                auto it = std::find_if(
                    m_tracksInStructure.begin(), m_tracksInStructure.end(),
                    [&currentPath](const TrackInStructure& tis) { return tis.m_pathToTrack == currentPath; });
                // Since the result is duplicated from the first field, the track must exist.
                assert(it != m_tracksInStructure.end() && "track not found for path");
                currentValue = std::move(it->m_track);
            } else if (const auto* recordType = currentType.as<babelwires::CompoundType>()) {
                const unsigned int numChildren = recordType->getNumChildren(currentValue);
                for (unsigned int i = 0; i < numChildren; ++i) {
                    auto [childValue, step, childTypeRef] = recordType->getChildNonConst(currentValue, i);
                    const babelwires::Type& childType = childTypeRef.resolve(m_typeSystem);
                    babelwires::Path newPath = currentPath;
                    newPath.pushStep(*step.asField());
                    assignTracksInStructure(childType, *childValue, newPath);
                }
            }
        }
    };
} // namespace

babelwires::TypeRef bw_music::getAccompanimentTypeForChords(
    const babelwires::TypeSystem& typeSystem, const babelwires::Type& typeOfAccompanimentTracks) {

    const babelwires::RecordType* recordType = typeOfAccompanimentTracks.as<babelwires::RecordType>();
    if (!recordType) {
        return {};
    }

    const auto& chordTypeType = typeSystem.getEntryByType<bw_music::ChordType>();
    for (auto f : recordType->getFields()) {
        const int enumIndex = chordTypeType.tryGetIndexFromIdentifier(f.m_identifier);
        if (enumIndex >= 0) {
            return f.m_type;
        }
    }
    // Error type.
    return {};
}

babelwires::ValueHolder bw_music::accompanimentSequencerFunction(
    const babelwires::TypeSystem& typeSystem, const babelwires::Type& typeOfAccompanimentTracks,
    const babelwires::ValueHolder& accompanimentTracks, const bw_music::Track& chordTrack) {

    const babelwires::RecordType* recordType = typeOfAccompanimentTracks.as<babelwires::RecordType>();
    assert(recordType && "accompanimentTracks must be of a record type");

    AccompanimentSequencer sequencer(typeSystem, *recordType, accompanimentTracks);
    sequencer.sequenceAccompaniment(chordTrack);
    //assert(sequencer.getResultTypeRef() == getAccompanimentTypeForChords(typeSystem, typeOfAccompanimentTracks, accompanimentTracks) &&
    //       "Result type does not match expected type");
    return sequencer.getResultValue();
}
