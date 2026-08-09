/**
 * Some common type definitions for musical data.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/pitch.hpp>
#include <MusicLib/Utilities/minMaxValue.hpp>

#include <BaseLib/Math/rational.hpp>
#include <BaseLib/common.hpp>

#include <cstdint>
#include <string>

namespace bw_music {

    using ModelDuration = babelwires::Rational ;
    
    /// The recommended storage type for musical velocities.
    /// Calculations should usually be performed with normalized doubles (i.e. in the range [0, 1.0]); this type is intended for storage and serialization.
    using VelocityStorage = MinMaxValue16;

} // namespace bw_music
