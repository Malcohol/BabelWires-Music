/**
 * Function which splits a track based on pitch.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Result/result.hpp>

#include <tuple>

namespace bw_music {
    struct SplitAtPitchResult {
        /// NoteEvents at or above the given pitch
        Track m_equalOrAbove;
        /// NoteEvents below the given pitch.
        Track m_below;
        /// Events which are not NoteEvents.
        Track m_other;
    };

    /// Split the events in the track by pitch.
    babelwires::ResultT<SplitAtPitchResult> splitAtPitch(Pitch pitch, const Track& sourceTrack);
} // namespace bw_music
