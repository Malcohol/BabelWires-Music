/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

namespace bw_music {
    /// Move the time at which events occur to the nearest beat.
    Track quantize(const Track& trackIn, ModelDuration beat);
}
