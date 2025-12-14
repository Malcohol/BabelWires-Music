/**
 * A ChordTypeSet stores a subset of chord types.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/chordTypeSet.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>

#include <cassert>

bw_music::ChordTypeSet::ChordTypeSet()
    : babelwires::ArrayType(ChordType::getThisType(), 1, static_cast<unsigned int>(ChordType::Value::NUM_VALUES), 1) {}

std::set<bw_music::ChordType::Value>
bw_music::ChordTypeSet::getChordTypesFromValue(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& value) const {
    assert(value && "ValueHolder must hold a value");
    assert(this->isValidValue(typeSystem, *value) && "ValueHolder must hold a valid value for this type");
    
    const auto& chordType = typeSystem.getRegisteredType(ChordType::getThisIdentifier()).is<ChordType>();
    std::set<bw_music::ChordType::Value> selectedChords;
    for (unsigned int i = 0; i < getNumChildren(value); ++i) {
        const auto [chordValueHolder, chordStep, chordChildType] = getChild(value, i);
        assert(chordChildType == ChordType::getThisType());
        const babelwires::ShortId chordId = (*chordValueHolder)->is<babelwires::EnumValue>().get();
        selectedChords.insert(chordType.getValueFromIdentifier(chordId));
    }
    return selectedChords;
}
