/**
 * A type used to represents generic accompaniment data.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

namespace babelwires {
    class TypeSystem;
    class TypeRef;
} // namespace babelwires

namespace bw_music {

    /// A RecordType whose field names are the chord types, and whose field types are type variables.
    /// Some processors use this to represent accompaniment data which can have contents of different types.
    babelwires::TypeRef getGenericAccompanimentTypeRef();

} // namespace bw_music
