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
    
    using Velocity = MinMaxValue16;

} // namespace bw_music
