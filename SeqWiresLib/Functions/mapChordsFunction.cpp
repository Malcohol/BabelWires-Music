/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/Functions/mapChordsFunction.hpp>

#include <SeqWiresLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <SeqWiresLib/Types/Track/TrackEvents/trackEventHolder.hpp>
#include <SeqWiresLib/chord.hpp>
#include <SeqWiresLib/pitch.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/addBlankToEnum.hpp>
#include <BabelWiresLib/Types/Enum/enumAtomTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumUnionTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/Helpers/enumValueAdapters.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/Types/Sum/sumType.hpp>
#include <BabelWiresLib/Types/Sum/sumTypeConstructor.hpp>
#include <BabelWiresLib/Types/Tuple/tupleType.hpp>
#include <BabelWiresLib/Types/Tuple/tupleTypeConstructor.hpp>
#include <BabelWiresLib/Types/Tuple/tupleValue.hpp>
#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

#include <optional>

babelwires::TypeRef seqwires::getMapChordFunctionSourceTypeRef() {
    return babelwires::SumTypeConstructor::makeTypeRef(
        {babelwires::TupleTypeConstructor::makeTypeRef(
             {babelwires::EnumUnionTypeConstructor::makeTypeRef(
                  babelwires::EnumAtomTypeConstructor::makeTypeRef(babelwires::getWildcardId()),
                  seqwires::PitchClass::getThisType()),
              babelwires::EnumUnionTypeConstructor::makeTypeRef(
                  babelwires::EnumAtomTypeConstructor::makeTypeRef(babelwires::getWildcardId()),
                  seqwires::ChordType::getThisType())}),
         seqwires::NoChord::getThisType()});
}

babelwires::TypeRef seqwires::getMapChordFunctionTargetTypeRef() {
    return babelwires::SumTypeConstructor::makeTypeRef(
        {babelwires::TupleTypeConstructor::makeTypeRef(
             {babelwires::EnumUnionTypeConstructor::makeTypeRef(
                  babelwires::EnumAtomTypeConstructor::makeTypeRef(babelwires::getWildcardMatchId()),
                  seqwires::PitchClass::getThisType()),
              babelwires::EnumUnionTypeConstructor::makeTypeRef(
                  babelwires::EnumAtomTypeConstructor::makeTypeRef(babelwires::getWildcardMatchId()),
                  seqwires::ChordType::getThisType())}),
         seqwires::NoChord::getThisType()});
}

namespace {

    // Since the types are not directly registered and are instead built using TypeConstructors, I have to
    // explicitly decompose them. Might be easier to register some of the pieces. Similarly, building the
    // enum from parts means I cannot use the native enum adapters and have to convert to and from indices
    // in the helper above.

    std::tuple<const babelwires::EnumType&, const babelwires::EnumType&>
    getSourceTupleComponentTypes(const babelwires::TypeSystem& typeSystem) {
        const babelwires::TypeRef sourceTypeRef = seqwires::getMapChordFunctionSourceTypeRef();
        const babelwires::SumType& sourceSumType = sourceTypeRef.resolve(typeSystem).is<babelwires::SumType>();
        assert(sourceSumType.getSummands().size() == 2);
        const babelwires::TypeRef& sourceTupleTypeRef = sourceSumType.getSummands()[0];
        const babelwires::TupleType& sourceTupleType =
            sourceTupleTypeRef.resolve(typeSystem).is<babelwires::TupleType>();
        assert(sourceTupleType.getComponentTypes().size() == 2);
        const babelwires::TypeRef& sourcePitchClassWCTypeRef = sourceTupleType.getComponentTypes()[0];
        const babelwires::EnumType& sourcePitchClassWCType =
            sourcePitchClassWCTypeRef.resolve(typeSystem).is<babelwires::EnumType>();
        const babelwires::TypeRef& sourceChordTypeWCTypeRef = sourceTupleType.getComponentTypes()[1];
        const babelwires::EnumType& sourceChordTypeWCType =
            sourceChordTypeWCTypeRef.resolve(typeSystem).is<babelwires::EnumType>();
        return {sourcePitchClassWCType, sourceChordTypeWCType};
    }

    std::tuple<const babelwires::EnumType&, const babelwires::EnumType&>
    getTargetTupleComponentTypes(const babelwires::TypeSystem& typeSystem) {
        const babelwires::TypeRef targetTypeRef = seqwires::getMapChordFunctionTargetTypeRef();
        const babelwires::SumType& targetSumType = targetTypeRef.resolve(typeSystem).is<babelwires::SumType>();
        assert(targetSumType.getSummands().size() == 2);
        const babelwires::TypeRef& targetTupleTypeRef = targetSumType.getSummands()[0];
        const babelwires::TupleType& targetTupleType =
            targetTupleTypeRef.resolve(typeSystem).is<babelwires::TupleType>();
        assert(targetTupleType.getComponentTypes().size() == 2);
        const babelwires::TypeRef& targetPitchClassWCTypeRef = targetTupleType.getComponentTypes()[0];
        const babelwires::EnumType& targetPitchClassWCType =
            targetPitchClassWCTypeRef.resolve(typeSystem).is<babelwires::EnumType>();
        const babelwires::TypeRef& targetChordTypeWCTypeRef = targetTupleType.getComponentTypes()[1];
        const babelwires::EnumType& targetChordTypeWCType =
            targetChordTypeWCTypeRef.resolve(typeSystem).is<babelwires::EnumType>();
        return {targetPitchClassWCType, targetChordTypeWCType};
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
        std::optional<seqwires::Chord> getNoChordTarget() {
            // Since the map is valid, the last entry only will be a fallback.
            for (int i = 0; i < m_mapValue.getNumMapEntries() - 1; ++i) {
                const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(i);
                if (entry.getSourceValue()->as<babelwires::EnumValue>()) {
                    assert(entry.getSourceValue()->as<babelwires::EnumValue>()->get() ==
                               seqwires::NoChord::getNoChordValue() &&
                           "Bare EnumValue in ChordMap source that wasn't the NoChord value");
                    if (entry.getTargetValue()->as<babelwires::EnumValue>()) {
                        assert(entry.getTargetValue()->as<babelwires::EnumValue>()->get() ==
                                   seqwires::NoChord::getNoChordValue() &&
                               "Bare EnumValue in ChordMap target that wasn't the NoChord value");
                        return {};
                    } else {
                        const babelwires::TupleValue& target = entry.getTargetValue()->is<babelwires::TupleValue>();
                        const unsigned int pitchClass =
                            m_targetPitchClassAdapter(target.getValue(0)->is<babelwires::EnumValue>());
                        const unsigned int chordType =
                            m_targetChordTypeAdapter(target.getValue(1)->is<babelwires::EnumValue>());
                        if ((pitchClass == m_indexOfPitchClassWildCardValue) ||
                            (chordType == m_indexOfChordTypeWildCardValue)) {
                            throw babelwires::ModelException() << "NoChord cannot be mapped to wildcard value";
                        }
                        return seqwires::Chord{seqwires::PitchClass::Value(pitchClass),
                                               seqwires::ChordType::Value(chordType)};
                    }
                }
            }
            // I think it would be expected that NoChord->NoChord if not specified, so the fallback is ignored.
            return {};
        };

        std::optional<seqwires::Chord> operator[](const seqwires::Chord& chord) {
            const std::uint16_t code = chordToCode(chord);
            const auto [it, wasInserted] = m_memo.try_emplace(code, -2);
            std::uint16_t& newCode = it->second;
            if (wasInserted) {
                unsigned int matchingEntry = 0;
                while (matchingEntry < m_mapValue.getNumMapEntries() - 1) {
                    const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(matchingEntry);
                    if (const babelwires::TupleValue* sourceTuple =
                            entry.getSourceValue()->as<babelwires::TupleValue>()) {
                        const unsigned int pitchClass =
                            m_targetPitchClassAdapter(sourceTuple->getValue(0)->is<babelwires::EnumValue>());
                        const unsigned int chordType =
                            m_targetChordTypeAdapter(sourceTuple->getValue(1)->is<babelwires::EnumValue>());
                        if (((static_cast<unsigned int>(chord.m_root) == pitchClass) ||
                             (pitchClass == m_indexOfPitchClassWildCardValue)) &&
                            ((static_cast<unsigned int>(chord.m_chordType) == chordType) ||
                             (chordType == m_indexOfChordTypeWildCardValue))) {
                            // Found match.
                            break;
                        }
                        ++matchingEntry;
                    }
                }
                const babelwires::MapEntryData& entry = m_mapValue.getMapEntry(matchingEntry);
                if (entry.getTargetValue()->as<babelwires::EnumValue>()) {
                    assert(entry.getTargetValue()->as<babelwires::EnumValue>()->get() ==
                               seqwires::NoChord::getNoChordValue() &&
                           "Bare EnumValue in ChordMap target that wasn't the NoChord value");
                    newCode = -1;
                } else {
                    const babelwires::TupleValue& target = entry.getTargetValue()->is<babelwires::TupleValue>();
                    unsigned int targetPitchClass =
                        m_targetPitchClassAdapter(target.getValue(0)->is<babelwires::EnumValue>());
                    seqwires::PitchClass::Value newPitchClass =
                        (targetPitchClass == m_indexOfPitchClassWildCardValue)
                            ? chord.m_root
                            : static_cast<seqwires::PitchClass::Value>(targetPitchClass);

                    unsigned int targetChordType =
                        m_targetChordTypeAdapter(target.getValue(1)->is<babelwires::EnumValue>());
                    seqwires::ChordType::Value newChordType =
                        (targetChordType == m_indexOfChordTypeWildCardValue)
                            ? chord.m_chordType
                            : static_cast<seqwires::ChordType::Value>(targetChordType);

                    newCode = chordToCode(seqwires::Chord(newPitchClass, newChordType));
                }
            }
            assert((newCode != -2) && "Entry not inserted");
            return codeToChord(newCode);
        }

      private:
        ChordMapApplicator(const babelwires::MapValue& mapValue,
                       std::tuple<const babelwires::EnumType&, const babelwires::EnumType&> sourceTupleComponentTypes,
                       std::tuple<const babelwires::EnumType&, const babelwires::EnumType&> targetTupleComponentTypes)
            : m_mapValue(mapValue)
            , m_sourcePitchClassAdapter(std::get<0>(sourceTupleComponentTypes))
            , m_sourceChordTypeAdapter(std::get<1>(sourceTupleComponentTypes))
            , m_targetPitchClassAdapter(std::get<0>(targetTupleComponentTypes))
            , m_targetChordTypeAdapter(std::get<1>(targetTupleComponentTypes))
            , m_indexOfPitchClassWildCardValue(std::get<0>(sourceTupleComponentTypes).getValueSet().size() - 1)
            , m_indexOfChordTypeWildCardValue(std::get<1>(sourceTupleComponentTypes).getValueSet().size() - 1) {
            assert(m_indexOfPitchClassWildCardValue == std::get<0>(targetTupleComponentTypes).getValueSet().size() - 1);
            assert(m_indexOfChordTypeWildCardValue == std::get<1>(targetTupleComponentTypes).getValueSet().size() - 1);
        }

        std::uint16_t chordToCode(const seqwires::Chord& chord) {
            return (static_cast<std::uint8_t>(chord.m_root) << 8) | static_cast<std::uint8_t>(chord.m_chordType);
        }

        std::optional<seqwires::Chord> codeToChord(std::uint16_t code) {
            if (code != -1) {
                return seqwires::Chord{static_cast<seqwires::PitchClass::Value>(code >> 8),
                                       static_cast<seqwires::ChordType::Value>(code & 255)};
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
        const unsigned int m_indexOfPitchClassWildCardValue;
        const unsigned int m_indexOfChordTypeWildCardValue;
    };
} // namespace

seqwires::Track seqwires::mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack,
                                            const babelwires::MapValue& chordMapValue) {
    if (!chordMapValue.isValid(typeSystem)) {
        throw babelwires::ModelException() << "The Chord Type Map is not valid.";
    }

    ChordMapApplicator mapApplicator(typeSystem, chordMapValue);

    Track trackOut;
    ModelDuration totalEventDuration;

    std::optional<seqwires::Chord> noChordChord = mapApplicator.getNoChordTarget();

    bool isChordPlaying = false;

    // Blank target handling:
    // If an event is dropped, then we need to carry its time forward for the next event.
    bool droppingChordEvent = false;
    ModelDuration timeSinceLastEvent;

    for (auto it = sourceTrack.begin(); it != sourceTrack.end(); ++it) {
        timeSinceLastEvent += it->getTimeSinceLastEvent();
        totalEventDuration += it->getTimeSinceLastEvent();

        if (noChordChord && !isChordPlaying && (timeSinceLastEvent > 0)) {
            trackOut.addEvent(ChordOnEvent(0, *noChordChord));
            isChordPlaying = true;
        }

        if (it->as<ChordOnEvent>()) {
            TrackEventHolder holder(*it);
            Chord& chord = holder->is<ChordOnEvent>().m_chord;
            if (std::optional<seqwires::Chord> targetChord = mapApplicator[chord]) {
                if (isChordPlaying) {
                    // Only if a noChordChord was added.
                    trackOut.addEvent(ChordOffEvent(timeSinceLastEvent));
                    timeSinceLastEvent = 0;
                }
                holder->setTimeSinceLastEvent(timeSinceLastEvent);
                timeSinceLastEvent = 0;
                chord = *targetChord;
                trackOut.addEvent(holder.release());
                isChordPlaying = true;
            } else {
                droppingChordEvent = true;
                isChordPlaying = false;
            }
        } else if (droppingChordEvent && it->as<ChordOffEvent>()) {
            droppingChordEvent = false;
        } else if (timeSinceLastEvent != it->getTimeSinceLastEvent()) {
            if (it->as<ChordOffEvent>()) {
                isChordPlaying = false;
            }
            TrackEventHolder holder(*it);
            holder->setTimeSinceLastEvent(timeSinceLastEvent);
            timeSinceLastEvent = 0;
            trackOut.addEvent(holder.release());
        } else {
            if (it->as<ChordOffEvent>()) {
                isChordPlaying = false;
            }
            trackOut.addEvent(*it);
            timeSinceLastEvent = 0;
        }
    }
    if (noChordChord && (totalEventDuration < sourceTrack.getDuration())) {
        trackOut.addEvent(ChordOnEvent(0, *noChordChord));
        trackOut.addEvent(ChordOffEvent(sourceTrack.getDuration() - totalEventDuration));
    }

    trackOut.setDuration(sourceTrack.getDuration());
    return trackOut;
}
