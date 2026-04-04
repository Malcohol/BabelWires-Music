/**
 * Some utility functions useful for manipulating music sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <optional>

namespace bw_music {
    class Track;

    /// Get the minimum denominator sufficient to represent the durations of all events in the track.
    MUSICLIB_API int getMinimumDenominator(const Track& track);

    /// How to treat pitches that go out of range when transposing.
    enum class TransposeOutOfRangePolicy {
        /// Pitches that go out of range are discarded.
        Discard,
        /// Pitches that go out of range are mapped to the nearest octave within range.
        MapToNearestOctave
    };

    /// Transpose a pitch by the given offset, according to the outOfRangePolicy.
    MUSICLIB_API std::optional<int> transposePitch(int pitch, int offset, TransposeOutOfRangePolicy outOfRangePolicy = TransposeOutOfRangePolicy::Discard, int lowerLimit = 0, int upperLimit = 127);

} // namespace bw_music
