/**
 * Provides some useful types and definitions for track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Utilities/minMaxValue.hpp>

namespace bw_music {
    /// The recommended storage type for musical velocities.
    /// Calculations should usually be performed with normalized doubles (i.e. in the range [0, 1.0]); this type is intended for storage and serialization.
    using VelocityStorage = MinMaxValue16;
}
