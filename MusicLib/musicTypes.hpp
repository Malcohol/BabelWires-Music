/**
 * Some common type definitions for musical data.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/pitch.hpp>

#include <Common/Math/rational.hpp>
#include <Common/types.hpp>

#include <cstdint>
#include <string>

namespace bw_music {

    typedef babelwires::Rational ModelDuration;
    typedef babelwires::Byte Velocity;

} // namespace bw_music
