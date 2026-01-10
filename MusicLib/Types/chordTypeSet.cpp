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

bw_music::ChordTypeSet::ChordTypeSet(const babelwires::TypeSystem& typeSystem)
    : babelwires::ArrayType(getThisIdentifier(), typeSystem.getEntryByType<ChordType>(), 0, static_cast<unsigned int>(ChordType::Value::NUM_VALUES), 1) {}

std::set<bw_music::ChordType::Value>
bw_music::ChordTypeSet::getChordTypesFromValue(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& value) const {
    assert(value && "ValueHolder must hold a value");
    assert(isValidValue(typeSystem, *value) && "ValueHolder must hold a valid value for this type");
    
    const auto& chordType = typeSystem.getEntryByType<ChordType>();
    std::set<bw_music::ChordType::Value> selectedChords;
    for (unsigned int i = 0; i < getNumChildren(value); ++i) {
        const auto [chordValueHolder, chordStep, chordChildType] = getChild(value, i);
        assert(chordChildType->getTypeExp() == ChordType::getThisIdentifier());
        const babelwires::ShortId chordId = (*chordValueHolder)->is<babelwires::EnumValue>().get();
        selectedChords.insert(chordType->getValueFromIdentifier(chordId));
    }
    return selectedChords;
}

babelwires::ValueHolder
bw_music::ChordTypeSet::createValueFromChordTypes(const babelwires::TypeSystem& typeSystem, const std::set<ChordType::Value>& chordTypes) const {
    babelwires::ValueHolder valueHolder = createValue(typeSystem);
    setSize(typeSystem, valueHolder, static_cast<unsigned int>(chordTypes.size()));
    const auto& chordTypeType = typeSystem.getEntryByType<ChordType>();
    unsigned int i = 0;
    for (auto chordType : chordTypes) {
        const auto [childValueHolder, childStep, childType] = getChildNonConst(valueHolder, i);
        const babelwires::ShortId chordId = chordTypeType->getIdentifierFromValue(chordType);
        *childValueHolder = babelwires::EnumValue(chordId);
        ++i;
    }
    return valueHolder;
}
