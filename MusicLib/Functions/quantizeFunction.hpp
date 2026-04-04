/**
 * Quantize track events.
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
    /// Move the time at which events occur to the nearest beat.
    MUSICLIB_API babelwires::ResultT<Track> quantize(const Track& trackIn, ModelDuration beat);
}
