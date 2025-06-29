/**
 * Function which adjusts a track of notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/primitiveType.hpp>

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/chord.hpp>

namespace bw_music {
    /// Adjust a track of notes to fit a chord.
    Track fitToChordFunction(const Track& sourceTrack, const ChordType::Value& chordType);

    // Produce a value the matches the input value, but adjusts any notes in any tracks to the given chord type.
    babelwires::ValueHolder fitToChordFunction(const babelwires::TypeSystem& typeSystem, const babelwires::Type& type, const babelwires::ValueHolder& sourceValue,
                                          const ChordType::Value& chordType);

} // namespace bw_music
