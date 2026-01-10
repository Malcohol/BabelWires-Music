/**
 * A ChordTypeSet stores a subset of chord types.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/chord.hpp>

#include <BabelWiresLib/Types/Array/arrayType.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>

#include <set>

namespace bw_music {

    /// ChordTypeSet stores a subset of chord types.
    /// Using an array for a set doesn't provide a great UI, but it will do for now.
    /// Having this registered type with the methods that work with a std::set means we can 
    /// change it later when a better representation is available.
    class ChordTypeSet : public babelwires::ArrayType {
      public:
        REGISTERED_TYPE("chordSet", "Chord Type Set", "e3593036-1a41-4a0d-9227-5d091106e16d", 1);

        ChordTypeSet(const babelwires::TypeSystem& typeSystem);
        
        /// Obtain the set of chords stored in the given value.
        std::set<ChordType::Value> getChordTypesFromValue(const babelwires::TypeSystem& typeSystem, const babelwires::ValueHolder& value) const;

        /// Create a value holding the given set of chord types.
        babelwires::ValueHolder createValueFromChordTypes(const babelwires::TypeSystem& typeSystem, const std::set<ChordType::Value>& chordTypes) const;
    };

} // namespace bw_music
