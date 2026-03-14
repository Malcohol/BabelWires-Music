/**
 * Function which merges tracks together.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Result/result.hpp>

namespace bw_music {
    /// Merge the events of the sourceTracks into targetTrack.
    babelwires::ResultT<Track> mergeTracks(const std::vector<const Track*>& sourceTracks);
} // namespace bw_music
