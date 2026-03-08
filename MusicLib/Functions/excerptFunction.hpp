/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Result/result.hpp>

#include <memory>

namespace bw_music {
    /// A function which extracts a section of sequence data from a track.
    /// Groups which start before the excerpt are dropped.
    /// Groups which finish after the excerpt are truncated.
    babelwires::ResultT<Track> getTrackExcerpt(const Track& trackIn, ModelDuration start, ModelDuration duration);
} // namespace bw_music
