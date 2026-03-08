/**
 * A function which creates a track with modified pitches.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Utilities/musicUtilities.hpp>

#include <BaseLib/Result/result.hpp>

#include <memory>

namespace bw_music {

    /// Return a track with the same events as trackIn, except the pitches have been adjusted.
    babelwires::ResultT<Track> transposeTrack(const Track& trackIn, int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy = TransposeOutOfRangePolicy::Discard);
} // namespace bw_music
