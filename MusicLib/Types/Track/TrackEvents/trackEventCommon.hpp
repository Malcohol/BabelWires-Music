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
#include <MusicLib/Utilities/minCentreMaxValue.hpp>

namespace bw_music {
    /// The recommended storage and serialization type for musical velocities.
    /// Calculations should usually be performed using normalized doubles (i.e. in the range [0, 1.0]).
    using VelocityStorage = MinMaxValue16;

    /// The recommended storage and serialization type for non-centred controllers (e.g. modulation wheel).
    /// Calculations should usually be performed using normalized doubles (i.e. in the range [0, 1.0]).
    using ControllerStorage = MinMaxValue32;

    /// The recommended storage and serialization type for centred controllers (e.g. pitch bend).
    /// Calculations should usually be performed using signed normalized doubles (i.e. in the range [-1.0, 1.0]).
    using CentredControllerStorage = MinCentreMaxValue32;
}
