/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/mapChordsFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/chord.hpp>
#include <MusicLib/pitch.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumAtomTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumUnionTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/Types/Sum/sumType.hpp>
#include <BabelWiresLib/Types/Sum/sumTypeConstructor.hpp>
#include <BabelWiresLib/Types/Tuple/tupleType.hpp>
#include <BabelWiresLib/Types/Tuple/tupleTypeConstructor.hpp>
#include <BabelWiresLib/Types/Tuple/tupleValue.hpp>
#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

#include <optional>

babelwires::TypeExp bw_music::getMapChordFunctionSourceTypeExp() {
    return babelwires::SumTypeConstructor::makeTypeExp(
        {babelwires::TupleTypeConstructor::makeTypeExp(
             {babelwires::EnumUnionTypeConstructor::makeTypeExp(
                  babelwires::EnumAtomTypeConstructor::makeTypeExp(babelwires::getWildcardId()),
                  bw_music::PitchClass::getThisType()),
              babelwires::EnumUnionTypeConstructor::makeTypeExp(
                  babelwires::EnumAtomTypeConstructor::makeTypeExp(babelwires::getWildcardId()),
                  bw_music::ChordType::getThisType())}),
         bw_music::NoChord::getThisType()});
}

babelwires::TypeExp bw_music::getMapChordFunctionTargetTypeExp() {
    return babelwires::SumTypeConstructor::makeTypeExp(
        {babelwires::TupleTypeConstructor::makeTypeExp(
             {babelwires::EnumUnionTypeConstructor::makeTypeExp(
                  babelwires::EnumAtomTypeConstructor::makeTypeExp(babelwires::getWildcardMatchId()),
                  bw_music::PitchClass::getThisType()),
              babelwires::EnumUnionTypeConstructor::makeTypeExp(
                  babelwires::EnumAtomTypeConstructor::makeTypeExp(babelwires::getWildcardMatchId()),
                  bw_music::ChordType::getThisType())}),
         bw_music::NoChord::getThisType()});
}

namespace {

    // Since the types are not directly registered and are instead built using TypeConstructors, I have to
    // explicitly decompose them. Might be easier to register some of the pieces. Similarly, building the
    // enum from parts means I cannot use the native enum adapters and have to convert to and from indices
    // in the helper above.

    std::tuple<const babelwires::TypePtrT<babelwires::EnumType>, const babelwires::TypePtrT<babelwires::EnumType>>
    decomposeMapTypes(const babelwires::TypeSystem& typeSystem, const babelwires::SumType& sumType) {
        assert(sumType.getSummands().size() == 2);
        const babelwires::TypePtr& sourceTupleTypeExp = sumType.getSummands()[0];
        const auto& tupleType = sourceTupleTypeExp->is<babelwires::TupleType>();
        assert(tupleType.getComponentTypes().size() == 2);
        const babelwires::TypePtr& pitchClassWCType = tupleType.getComponentTypes()[0];
        const babelwires::TypePtr& chordTypeWCType = tupleType.getComponentTypes()[1];
        return {std::move(typeAs<babelwires::EnumType>(pitchClassWCType)), std::move(typeAs<babelwires::EnumType>(chordTypeWCType))};
    }

    std::tuple<const babelwires::TypePtrT<babelwires::EnumType>, const babelwires::TypePtrT<babelwires::EnumType>>
    getSourceTupleComponentTypes(const babelwires::TypeSystem& typeSystem) {
        const babelwires::TypeExp sourceTypeExp = bw_music::getMapChordFunctionSourceTypeExp();
        const auto& sourceSumType = sourceTypeExp.resolveAs<babelwires::SumType>(typeSystem);
        return decomposeMapTypes(typeSystem, *sourceSumType);
    }

    std::tuple<const babelwires::TypePtrT<babelwires::EnumType>, const babelwires::TypePtrT<babelwires::EnumType>>
    getTargetTupleComponentTypes(const babelwires::TypeSystem& typeSystem) {
        const babelwires::TypeExp targetTypeExp = bw_music::getMapChordFunctionTargetTypeExp();
        const auto& targetSumType = targetTypeExp.resolveAs<babelwires::SumType>(typeSystem);
        return decomposeMapTypes(typeSystem, *targetSumType);
    }

    /// As yet, there is no generic handling of wildcards when they occur within tuples, as in this case.
    /// Therefore, I cannot use one of the preexisting applicators.
    /// TODO: Can this be made generic?
    class ChordMapApplicator {
      public:
        ChordMapApplicator(const babelwires::TypeSystem& typeSystem, const babelwires::MapValue& mapValue)
            : ChordMapApplicator(mapValue, getSourceTupleComponentTypes(typeSystem),
                                 getTargetTupleComponentTypes(typeSystem)) {}

        /// This isn't memoized, so the caller should remember it.
        std::optional<bw_music::Chord> getNoChordTarget() {
            // Since the map is valid, the last entry only will be a fallback.
            for (int i = 0; i < m_mapValue.getNumMapEntries() - 1; ++i) {
                const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(i);
                if (entry.getSourceValue()->as<babelwires::EnumValue>()) {
                    assert(entry.getSourceValue()->as<babelwires::EnumValue>()->get() ==
                               bw_music::NoChord::getNoChordValue() &&
                           "Bare EnumValue in ChordMap source that wasn't the NoChord value");
                    if (entry.getTargetValue()->as<babelwires::EnumValue>()) {
                        assert(entry.getTargetValue()->as<babelwires::EnumValue>()->get() ==
                                   bw_music::NoChord::getNoChordValue() &&
                               "Bare EnumValue in ChordMap target that wasn't the NoChord value");
                        return {};
                    } else {
                        const babelwires::TupleValue& target = entry.getTargetValue()->is<babelwires::TupleValue>();
                        const unsigned int pitchClassIndex =
                            m_targetPitchClassAdapter(target.getValue(0)->is<babelwires::EnumValue>());
                        const unsigned int chordTypeIndex =
                            m_targetChordTypeAdapter(target.getValue(1)->is<babelwires::EnumValue>());
                        if ((pitchClassIndex == 0) || (chordTypeIndex == 0)) {
                            throw babelwires::ModelException() << "NoChord cannot be mapped to wildcard value";
                        }
                        return bw_music::Chord{bw_music::PitchClass::Value(pitchClassIndex - 1),
                                               bw_music::ChordType::Value(chordTypeIndex - 1)};
                    }
                }
            }
            // I think it would be expected that NoChord->NoChord if not specified, so the fallback is ignored.
            return {};
        };

        std::optional<bw_music::Chord> operator[](const bw_music::Chord& chord) {
            const std::uint16_t code = chordToCode(chord);
            const auto [it, wasInserted] = m_memo.try_emplace(code, -2);
            std::uint16_t& newCode = it->second;
            if (wasInserted) {
                unsigned int matchingEntry = 0;
                while (matchingEntry < m_mapValue.getNumMapEntries() - 1) {
                    const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(matchingEntry);
                    if (const babelwires::TupleValue* sourceTuple =
                            entry.getSourceValue()->as<babelwires::TupleValue>()) {
                        const unsigned int pitchClassIndex =
                            m_sourcePitchClassAdapter(sourceTuple->getValue(0)->is<babelwires::EnumValue>());
                        const unsigned int chordTypeIndex =
                            m_sourceChordTypeAdapter(sourceTuple->getValue(1)->is<babelwires::EnumValue>());
                        if (((static_cast<unsigned int>(chord.m_root) + 1 == pitchClassIndex) ||
                             (pitchClassIndex == 0)) &&
                            ((static_cast<unsigned int>(chord.m_chordType) + 1 == chordTypeIndex) ||
                             (chordTypeIndex == 0))) {
                            // Found match.
                            break;
                        }
                    }
                    ++matchingEntry;
                }
                const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(matchingEntry);
                if (entry.getTargetValue()->as<babelwires::EnumValue>()) {
                    assert(entry.getTargetValue()->as<babelwires::EnumValue>()->get() ==
                               bw_music::NoChord::getNoChordValue() &&
                           "Bare EnumValue in ChordMap target that wasn't the NoChord value");
                    newCode = -1;
                } else {
                    const babelwires::TupleValue& target = entry.getTargetValue()->is<babelwires::TupleValue>();
                    unsigned int targetPitchClassIndex =
                        m_targetPitchClassAdapter(target.getValue(0)->is<babelwires::EnumValue>());
                    bw_music::PitchClass::Value newPitchClass =
                        (targetPitchClassIndex == 0)
                            ? chord.m_root
                            : static_cast<bw_music::PitchClass::Value>(targetPitchClassIndex - 1);

                    unsigned int targetChordTypeIndex =
                        m_targetChordTypeAdapter(target.getValue(1)->is<babelwires::EnumValue>());
                    bw_music::ChordType::Value newChordType =
                        (targetChordTypeIndex == 0) ? chord.m_chordType
                                                    : static_cast<bw_music::ChordType::Value>(targetChordTypeIndex - 1);

                    newCode = chordToCode(bw_music::Chord(newPitchClass, newChordType));
                }
            }
            assert((newCode != -2) && "Entry not inserted");
            return codeToChord(newCode);
        }

      private:
        ChordMapApplicator(
            const babelwires::MapValue& mapValue,
            std::tuple<const babelwires::TypePtrT<babelwires::EnumType>, const babelwires::TypePtrT<babelwires::EnumType>> sourceTupleComponentTypes,
            std::tuple<const babelwires::TypePtrT<babelwires::EnumType>, const babelwires::TypePtrT<babelwires::EnumType>> targetTupleComponentTypes)
            : m_mapValue(mapValue)
            , m_sourcePitchClassAdapter(std::get<0>(sourceTupleComponentTypes))
            , m_sourceChordTypeAdapter(std::get<1>(sourceTupleComponentTypes))
            , m_targetPitchClassAdapter(std::get<0>(targetTupleComponentTypes))
            , m_targetChordTypeAdapter(std::get<1>(targetTupleComponentTypes)) {}

        std::uint16_t chordToCode(const bw_music::Chord& chord) const {
            return (static_cast<std::uint8_t>(chord.m_root) << 8) | static_cast<std::uint8_t>(chord.m_chordType);
        }

        std::optional<bw_music::Chord> codeToChord(std::uint16_t code) const {
            if (code != static_cast<std::uint16_t>(-1)) {
                return bw_music::Chord{static_cast<bw_music::PitchClass::Value>(code >> 8),
                                       static_cast<bw_music::ChordType::Value>(code & 255)};
            } else {
                return {};
            }
        }

      private:
        std::unordered_map<std::uint16_t, std::uint16_t> m_memo;
        const babelwires::MapValue& m_mapValue;
        babelwires::EnumToIndexValueAdapter m_sourcePitchClassAdapter;
        babelwires::EnumToIndexValueAdapter m_sourceChordTypeAdapter;
        babelwires::EnumToIndexValueAdapter m_targetPitchClassAdapter;
        babelwires::EnumToIndexValueAdapter m_targetChordTypeAdapter;
    };
} // namespace

bw_music::Track bw_music::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                            const babelwires::MapValue& chordMapValue) {
    if (!chordMapValue.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    ChordMapApplicator mapApplicator(typeSystem, chordMapValue);

    TrackBuilder trackOut;
    ModelDuration totalEventDuration;

    std::optional<bw_music::Chord> silenceToChordChord = mapApplicator.getNoChordTarget();

    enum {
        pending,
        chordToChord,
        silenceToChord,
        chordToSilence,
        chordToSilencePending,
    } state = pending;

    ModelDuration timeSinceLastEvent;

    for (auto it = sourceTrack.begin(); it != sourceTrack.end(); ++it) {
        timeSinceLastEvent += it->getTimeSinceLastEvent();
        totalEventDuration += it->getTimeSinceLastEvent();

        if (silenceToChordChord && (state == pending) && (timeSinceLastEvent > 0)) {
            trackOut.addEvent(ChordOnEvent(0, *silenceToChordChord));
            state = silenceToChord;
        }

        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            if (std::optional<bw_music::Chord> targetChord = mapApplicator[chord]) {
                if (state == silenceToChord) {
                    trackOut.addEvent(ChordOffEvent(timeSinceLastEvent));
                    timeSinceLastEvent = 0;
                }
                holder->setTimeSinceLastEvent(timeSinceLastEvent);
                timeSinceLastEvent = 0;
                chord = *targetChord;
                trackOut.addEvent(holder.release());
                state = chordToChord;
            } else {
                state = chordToSilence;
            }
        } else if ((state == chordToSilence) && it->as<ChordOffEvent>()) {
            state = chordToSilencePending;
        } else {
            if (it->as<ChordOffEvent>()) {
                state = pending;
            }
            // Write the event.
            if (timeSinceLastEvent != it->getTimeSinceLastEvent()) {
                TrackEventHolder holder(*it);
                holder->setTimeSinceLastEvent(timeSinceLastEvent);
                timeSinceLastEvent = 0;
                trackOut.addEvent(holder.release());
            } else {
                trackOut.addEvent(*it);
                timeSinceLastEvent = 0;
            }
        }
    }
    timeSinceLastEvent += sourceTrack.getDuration() - totalEventDuration;
    if (silenceToChordChord && (state == pending) && (timeSinceLastEvent > 0)) {
        trackOut.addEvent(ChordOnEvent(0, *silenceToChordChord));
        state = silenceToChord;
    }
    if (state == silenceToChord) {
        trackOut.addEvent(ChordOffEvent(timeSinceLastEvent));
    }

    return trackOut.finishAndGetTrack(sourceTrack.getDuration());
}
