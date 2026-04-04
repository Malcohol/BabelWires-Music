/**
 * Function which adjusts a track of notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/chord.hpp>

#include <BaseLib/Result/result.hpp>

namespace bw_music {
    /// Adjust a track of notes to fit a chord, but adjusts any notes in any tracks to the given chord type.
    /// The input is assumed notes are assumed to be in C major.
    MUSICLIB_API babelwires::ResultT<Track> fitToChordFunction(const Track& sourceTrack, const Chord& chord);

} // namespace bw_music
