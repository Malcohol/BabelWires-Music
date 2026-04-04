/**
 * A function which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Result/result.hpp>

#include <memory>

namespace bw_music {
    /// A function which repeats a section of sequence data a number of times.
    MUSICLIB_API babelwires::ResultT<Track> repeatTrack(const Track& trackIn, int count);
} // namespace bw_music
