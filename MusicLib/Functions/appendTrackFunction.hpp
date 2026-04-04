/**
 * Function which join tracks together sequencially.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Result/result.hpp>

namespace bw_music {
    /// Add the events of sourceTrack to the end of targetTrack.
    MUSICLIB_API babelwires::Result appendTrack(Track& targetTrack, const Track& sourceTrack);
} // namespace bw_music
