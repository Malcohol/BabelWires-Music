/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/Utilities/musicUtilities.hpp>
#include <MusicLib/Utilities/trackTraverser.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>

#include <BaseLib/Log/userLogger.hpp>

#include <algorithm>
#include <set>
#include <sstream>

namespace {
    // See page 237 of the SC-8850 English manual for the part to block conversion.
    // We will always use the default part mapping, where parts correspond to midi channels.
    const std::array<unsigned int, 16> s_gsChannelToBlockMapping{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 11, 12, 13, 14, 15};
} // namespace

smf::SmfWriter::SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                          const babelwires::ValueTreeRoot& sequence, std::ostream& ostream)
    : m_projectContext(projectContext)
    , m_userLogger(userLogger)
    , m_smfFeature(sequence)
    , m_ostream(ostream)
    , m_os(&m_ostream)
    , m_division(256)
    , m_standardPercussionSets(projectContext) {}

void smf::SmfWriter::writeUint16(std::uint16_t i) {
    m_os->put(i >> 8);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeUint24(std::uint32_t i) {
    assert((i < (1 << 24)) && "Value cannot be represented in 24 bits");
    m_os->put(i >> 16);
    m_os->put((i >> 8) & 255);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeUint32(std::uint32_t i) {
    m_os->put(i >> 24);
    m_os->put((i >> 16) & 255);
    m_os->put((i >> 8) & 255);
    m_os->put(i & 255);
}

void smf::SmfWriter::writeVariableLengthQuantity(std::uint32_t i) {
    assert((i <= 0x0fffffff) && "Value is too big for a variable-lengths quantity");

    if (const std::uint32_t b0 = ((i >> 21) & 0x7f)) {
        m_os->put(b0 | 0x80);
    }
    if (const std::uint32_t b1 = ((i >> 14) & 0x7f)) {
        m_os->put(b1 | 0x80);
    }
    if (const std::uint32_t b2 = ((i >> 7) & 0x7f)) {
        m_os->put(b2 | 0x80);
    }
    const std::uint32_t b3 = i & 0x7f;
    m_os->put(b3);
}

void smf::SmfWriter::writeModelDuration(const bw_music::ModelDuration& d) {
    // TODO: Quantize if this isn't exact.
    const int numDivisions = (d.getNumerator() * m_division * 4) / (d.getDenominator());
    writeVariableLengthQuantity(numDivisions);
}

void smf::SmfWriter::writeTempoEvent(int bpm) {
    m_os->put(0x00u);
    m_os->put(0xffu);
    m_os->put(0x51u);
    m_os->put(0x03u);

    const int d = 60'000'000 / bpm;

    writeUint24(d);
}

void smf::SmfWriter::writeTextMetaEvent(int type, std::string text) {
    assert((0 <= type) && (type <= 15) && "Type is out-of-range.");
    babelwires::Byte t = type;
    m_os->put(0x00u);
    m_os->put(0xffu);
    m_os->put(t);
    writeVariableLengthQuantity(text.length());

    // TODO assert text is ASCII.
    *m_os << text;
}

smf::SmfSequence::ConstInstance smf::SmfWriter::getSmfSequenceConst() const {
    return babelwires::FileTypeT<SmfSequence>::ConstInstance(m_smfFeature).getConts();
}

void smf::SmfWriter::writeHeaderChunk(unsigned int numTracks) {
    const auto& smfType = getSmfSequenceConst();

    assert((m_division < (2 << 15)) && "division is too large");

    const unsigned int tagIndex = smfType.getInstanceType().getIndexOfTag(smfType.getSelectedTag());

    m_os->write("MThd", 4);
    writeUint32(6);
    writeUint16(tagIndex);
    writeUint16((tagIndex == 0) ? 1 : numTracks);

    {
        int division = 1;
        applyToAllTracks([&division](unsigned int channelNumber, const bw_music::Track& track) {
            division = babelwires::lcm(division, bw_music::getMinimumDenominator(track));
        });
        m_division = division;
    }

    // 0 in high-bit implies metrical.
    writeUint16(m_division);
}

smf::SmfWriter::WriteTrackEventResult smf::SmfWriter::writeTrackEvent(int channelNumber,
                                                                      bw_music::ModelDuration timeSinceLastEvent,
                                                                      const bw_music::TrackEvent& e) {
    assert(channelNumber >= 0);
    assert(channelNumber <= 15);

    if (const bw_music::PercussionSetWithPitchMap* const kitIfPercussion =
            m_channelSetup[channelNumber].m_kitIfPercussion) {
        if (const bw_music::PercussionOnEvent* percussionOn = e.tryAs<bw_music::PercussionOnEvent>()) {
            if (auto maybePitch = kitIfPercussion->tryGetPitchFromInstrument(percussionOn->getInstrument())) {
                writeModelDuration(timeSinceLastEvent);
                m_os->put(0b10010000 | channelNumber);
                m_os->put(*maybePitch);
                m_os->put(percussionOn->getVelocity());
                return WriteTrackEventResult::Written;
            } else {
                return WriteTrackEventResult::NotInPercussionSet;
            }
        } else if (const bw_music::PercussionOffEvent* percussionOff = e.tryAs<bw_music::PercussionOffEvent>()) {
            if (auto maybePitch = kitIfPercussion->tryGetPitchFromInstrument(percussionOff->getInstrument())) {
                writeModelDuration(timeSinceLastEvent);
                m_os->put(0b10000000 | channelNumber);
                m_os->put(*maybePitch);
                m_os->put(percussionOff->getVelocity());
                return WriteTrackEventResult::Written;
            } else {
                return WriteTrackEventResult::NotInPercussionSet;
            }
        }
    } else {
        if (const bw_music::NoteOnEvent* noteOn = e.tryAs<bw_music::NoteOnEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10010000 | channelNumber);
            m_os->put(noteOn->m_pitch);
            m_os->put(noteOn->m_velocity);
            return WriteTrackEventResult::Written;
        } else if (const bw_music::NoteOffEvent* noteOff = e.tryAs<bw_music::NoteOffEvent>()) {
            writeModelDuration(timeSinceLastEvent);
            m_os->put(0b10000000 | channelNumber);
            m_os->put(noteOff->m_pitch);
            m_os->put(noteOff->m_velocity);
            return WriteTrackEventResult::Written;
        }
    }
    return WriteTrackEventResult::WrongCategory;
}

void smf::SmfWriter::writeNotes(const std::vector<ChannelAndTrack>& tracks) {
    const int numTracks = tracks.size();

    bw_music::ModelDuration trackDuration = 0;

    // TODO Use the mergeFunction to multiplex the channels.

    std::vector<bw_music::TrackTraverser<bw_music::FilteredTrackIterator<bw_music::TrackEvent>>> traversers;

    for (int i = 0; i < numTracks; ++i) {
        const bw_music::Track& track = *std::get<1>(tracks[i]);
        traversers.emplace_back(track, bw_music::iterateOver<bw_music::TrackEvent>(track));
        traversers.back().leastUpperBoundDuration(trackDuration);
    }

    bw_music::ModelDuration timeSinceStart = 0;
    bw_music::ModelDuration timeOfLastEvent = 0;
    while (timeSinceStart < trackDuration) {
        bw_music::ModelDuration timeToNextEvent = trackDuration - timeSinceStart;
        for (int i = 0; i < numTracks; ++i) {
            traversers[i].greatestLowerBoundNextEvent(timeToNextEvent);
        }

        bool isFirstEventAtThisTime = true;
        for (int i = 0; i < numTracks; ++i) {
            const unsigned int channelNumber = std::get<0>(tracks[i]);
            traversers[i].advance(timeToNextEvent, [this, &isFirstEventAtThisTime, &timeToNextEvent, &timeOfLastEvent,
                                                    &timeSinceStart, channelNumber](const bw_music::TrackEvent& event) {
                const bw_music::ModelDuration timeToThisEvent = isFirstEventAtThisTime ? timeToNextEvent : 0;
                const WriteTrackEventResult result = writeTrackEvent(channelNumber, timeToThisEvent, event);
                if (result == WriteTrackEventResult::Written) {
                    timeOfLastEvent = timeSinceStart + timeToNextEvent;
                    isFirstEventAtThisTime = false;
                } else {
                    // TODO Warn user about events which could not be written.
                    m_userLogger.logWarning() << "Event could not be written";
                }
            });
        }

        timeSinceStart += timeToNextEvent;
    }

    // End of track event.
    writeModelDuration(trackDuration - timeOfLastEvent);
}

template <std::size_t N> void smf::SmfWriter::writeMessage(const std::array<std::uint8_t, N>& message) {
    for (int i = 0; i < message.size(); ++i) {
        m_os->put(message[i]);
    }
}

void smf::SmfWriter::writeGlobalSetup() {
    const auto& metadata = getSmfSequenceConst().getMeta();

    switch (metadata.getSpec().get()) {
        case GMSpecType::Value::GM:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 7>{0b11110000, 0x05, 0x7E, 0x7F, 0x09, 0x01, 0xF7});
            break;
        case GMSpecType::Value::GM2:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 7>{0b11110000, 0x05, 0x7E, 0x7F, 0x09, 0x03, 0xF7});
            break;
        case GMSpecType::Value::GS:
            writeModelDuration(0);
            writeMessage(std::array<std::uint8_t, 12>{0b11110000, 0x0A, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00,
                                                      0x41, 0xF7});
            break;
        case GMSpecType::Value::XG:
            writeModelDuration(0);
            writeMessage(
                std::array<std::uint8_t, 10>{0b11110000, 0x08, 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7});
        default:
        case GMSpecType::Value::NONE:
            break;
    }

    if (const auto& copyright = metadata.tryGetCopyR()) {
        if (!copyright->get().empty()) {
            writeTextMetaEvent(2, copyright->get());
        }
    }
    if (const auto& sequenceOrTrackName = metadata.tryGetName()) {
        if (!sequenceOrTrackName->get().empty()) {
            writeTextMetaEvent(3, sequenceOrTrackName->get());
        }
    }

    if (const auto& tempo = metadata.tryGetTempo()) {
        writeTempoEvent(tempo->get());
    }
}

void smf::SmfWriter::writeTrack(const std::vector<ChannelAndTrack>& tracks, bool includeGlobalSetup) {
    std::ostream* oldStream = m_os;
    std::ostringstream tempStream;
    m_os = &tempStream;

    if (includeGlobalSetup) {
        writeGlobalSetup();
    }

    const GMSpecType::Value gmSpec = getSmfSequenceConst().getMeta().getSpec().get();

    for (int i = 0; i < tracks.size(); ++i) {
        const unsigned int channelNumber = std::get<0>(tracks[i]);
        ChannelSetup& channelSetup = m_channelSetup[channelNumber];
        if (!channelSetup.m_setupWritten) {
            const std::optional<StandardPercussionSets::ChannelSetupInfo> info =
                m_standardPercussionSets.getChannelSetupInfoFromPercussionSet(channelSetup.m_kitIfPercussion,
                                                                              channelNumber);
            if (info) {
                if (gmSpec == GMSpecType::Value::GS) {
                    // Set GS "Use For Rhythm Part"
                    writeModelDuration(0);
                    const std::uint8_t block = 0x10 | s_gsChannelToBlockMapping[channelNumber];
                    const std::uint8_t checksum = (0x80 - ((0x40 + block + 0x15 + info->m_gsPartMode) % 0x80)) % 0x80;
                    writeMessage(std::array<std::uint8_t, 12>{0b11110000, 0x0A, 0x41, 0x10, 0x42, 0x12, 0x40, block,
                                                              0x15, info->m_gsPartMode, checksum, 0xF7});
                } else {
                    // Bank select MSB
                    writeModelDuration(0);
                    writeMessage(std::array<std::uint8_t, 3>{static_cast<std::uint8_t>(0b10110000 | channelNumber),
                                                             0x00, info->m_bankMSB});

                    // Bank select LSB
                    writeModelDuration(0);
                    writeMessage(std::array<std::uint8_t, 3>{static_cast<std::uint8_t>(0b10110000 | channelNumber),
                                                             0x20, info->m_bankLSB});
                }

                // Program change.
                writeModelDuration(0);
                writeMessage(std::array<std::uint8_t, 2>{static_cast<std::uint8_t>(0b11000000 | channelNumber),
                                                         info->m_program});
            }
            channelSetup.m_setupWritten = true;
        }
    }

    writeNotes(tracks);

    // End of track.
    m_os->put(0xffu);
    m_os->put(0x2Fu);
    m_os->put(0x00u);

    m_os = oldStream;
    m_os->write("MTrk", 4);
    writeUint32(static_cast<std::uint32_t>(tempStream.tellp()));
    m_os->write(tempStream.str().data(), tempStream.tellp());
}

void smf::SmfWriter::setUpPercussionKit(const std::unordered_set<babelwires::ShortId>& instrumentsInUse,
                                        int channelNumber) {
    const GMSpecType::Value gmSpec = getSmfSequenceConst().getMeta().getSpec().get();
    std::unordered_set<babelwires::ShortId> excludedInstruments;
    m_channelSetup[channelNumber].m_kitIfPercussion =
        m_standardPercussionSets.getBestPercussionSet(gmSpec, channelNumber, instrumentsInUse, excludedInstruments);
    if (!excludedInstruments.empty()) {
        m_userLogger.logWarning() << "Percussion events for " << excludedInstruments.size()
                                  << " instruments could not be represented in channel " << channelNumber;
    }
}

namespace {
    void getPercussionInstrumentsInUse(const bw_music::Track& track,
                                       std::unordered_set<babelwires::ShortId>& instrumentsInUse) {
        for (auto it : bw_music::iterateOver<bw_music::PercussionEvent>(track)) {
            instrumentsInUse.insert(it.getInstrument());
        }
    }

} // namespace

void smf::SmfWriter::setUpPercussionSets() {
    std::array<std::unordered_set<babelwires::ShortId>, 16> instrumentsInUse;
    applyToAllTracks([this, &instrumentsInUse](unsigned int channelNumber, const bw_music::Track& track) {
        getPercussionInstrumentsInUse(track, instrumentsInUse[channelNumber]);
    });
    for (int i = 0; i < 16; ++i) {
        setUpPercussionKit(instrumentsInUse[i], i);
    }
}

void smf::SmfWriter::applyToAllTracks(std::function<void(unsigned int, const bw_music::Track&)> func) {
    const auto& smfType = getSmfSequenceConst();
    if (smfType.getInstanceType().getIndexOfTag(smfType.getSelectedTag()) == 0) {
        const auto& tracks = smfType.getTrcks0();
        for (unsigned int c = 0; c < 16; ++c) {
            if (auto track = tracks.tryGetTrack(c)) {
                func(c, track->get());
            }
        }
    } else {
        const auto& tracks = smfType.getTrcks1();
        const int numTracks = tracks.getSize();
        for (int i = 0; i < numTracks; ++i) {
            auto entry = tracks.getEntry(i);
            func(entry.getChan().get(), entry.getTrack().get());
        }
    }
}

void smf::SmfWriter::write() {
    setUpPercussionSets();

    std::vector<ChannelAndTrack> channelAndTrackValues;

    const auto& smfType = getSmfSequenceConst();

    if (smfType.getInstanceType().getIndexOfTag(smfType.getSelectedTag()) == 0) {
        const auto& tracks = smfType.getTrcks0();
        for (unsigned int c = 0; c < 16; ++c) {
            if (auto track = tracks.tryGetTrack(c)) {
                channelAndTrackValues.emplace_back(ChannelAndTrack{c, &track->get()});
            }
        }
        writeHeaderChunk(channelAndTrackValues.size());
        writeTrack(channelAndTrackValues, true);
    } else {
        const auto& tracks = smfType.getTrcks1();
        const int numTracks = tracks.getSize();
        writeHeaderChunk(numTracks);
        for (int i = 0; i < numTracks; ++i) {
            channelAndTrackValues.clear();
            auto trackAndChannel = tracks.getEntry(i);
            channelAndTrackValues.emplace_back(
                ChannelAndTrack{trackAndChannel.getChan().get(), &trackAndChannel.getTrack().get()});
            for (unsigned int c = 0; c < 16; ++c) {
                if (auto extraTrack = trackAndChannel.tryGetTrack(c)) {
                    channelAndTrackValues.emplace_back(ChannelAndTrack{c, &extraTrack->get()});
                }
            }
            writeTrack(channelAndTrackValues, (i == 0));
        }
    }
}

void smf::writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                     const babelwires::ValueTreeRoot& sequence, std::ostream& output) {
    smf::SmfWriter writer(projectContext, userLogger, sequence, output);
    writer.write();
}
