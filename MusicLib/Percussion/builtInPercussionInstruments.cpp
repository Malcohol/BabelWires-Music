/**
 * Defines enums for standard percussion sets.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>

#include <MusicLib/Percussion/percussionTypeTag.hpp>

ENUM_DEFINE_ENUM_VALUE_SOURCE(bw_music::BuiltInPercussionInstruments, BUILT_IN_PERCUSSION_INSTRUMENTS);

bw_music::BuiltInPercussionInstruments::BuiltInPercussionInstruments()
    : EnumType(getThisIdentifier(), getStaticValueSet(), 0) {
        addTag(percussionTypeTag());
    }

