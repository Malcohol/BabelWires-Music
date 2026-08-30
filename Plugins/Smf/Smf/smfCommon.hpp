/**
 * Some definitions for use across the plugin.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackType.hpp>

namespace smf {
    using TypeOfTracks = bw_music::DefaultTrackType;

    constexpr babelwires::Byte c_bankSelectMsbController = 0x00;
    constexpr babelwires::Byte c_volumeMsbController = 0x07;
    constexpr babelwires::Byte c_panMsbController = 0x0A;
    constexpr babelwires::Byte c_expressionMsbController = 0x0B;
    constexpr babelwires::Byte c_expressionLsbController = 0x2B;
    constexpr babelwires::Byte c_bankSelectLsbController = 0x20;
    constexpr babelwires::Byte c_sustainController = 0x40;
}
