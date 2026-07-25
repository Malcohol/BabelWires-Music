/**
 * Some utility functions useful for scaling between values of different resolutions, such as MIDI 1.0 and MIDI 2.0.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/Result/result.hpp>

#include <cstdint>

namespace bw_music {
    /// Scale the source value from a smaller to a larger domain of values using the MIDI 2.0 "min-centre-max" scaling
    /// scheme.
    /// Assumes that the domain of source values is the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1]
    /// and that the domain of dest values is the range [0,.. 2^(numDestBits - 1),.. (2^numDestBits) - 1].
    template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
    constexpr std::uint32_t minCentreMaxScaleUp(std::uint32_t sourceValue);

    /// Scale the source value from a larger to a smaller domain of values using the MIDI 2.0 "min-centre-max" scaling
    /// scheme.
    /// Assumes that the domain of source values is the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1]
    /// and that the domain of dest values is the range [0,.. 2^(numDestBits - 1),.. (2^numDestBits) - 1].
    template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
    constexpr std::uint32_t minCentreMaxScaleDown(std::uint32_t sourceValue);

    /// Scale from a 7-bit value ([0,.. 64,.. 127]) to a 32-bit value ([0,.. 0x80000000,.. 0xFFFFFFFF].
    MUSICLIB_API std::uint32_t minCentreMaxScale7to32(std::uint8_t lowResValue);

    /// Scale from a 32-bit value ([0,.. 0x80000000,.. 0xFFFFFFFF]) to a 7-bit value ([0,.. 64,.. 127]).
    MUSICLIB_API std::uint8_t minCentreMaxScale32to7(std::uint32_t highResValue);

    /// Scale a signed normalized value in the range ([-1.0,.. 0.0,.. 1.0]) to a 32-bit value ([0,.. 0x80000000,..
    /// 0xFFFFFFFF]). Fails if the input value is outside the range.
    MUSICLIB_API babelwires::ResultT<std::uint32_t> scaleSignedNormalizedDoubleto32(double signedNormalizedValue);

    /// Scale a signed normalized value in the range ([-1.0,.. 0.0,.. 1.0]) to a 32-bit value ([0,.. 0x80000000,..
    /// 0xFFFFFFFF]). Asserts that the input value is within the range.
    MUSICLIB_API std::uint32_t assertScaleSignedNormalizedDoubleto32(double signedNormalizedValue);

    /// Scale a signed normalized value in the range ([-1.0,.. 0.0,.. 1.0]) to a 32-bit value ([0,.. 0x80000000,..
    /// 0xFFFFFFFF]). Clamps the output value to the range if the input value is outside the range.
    MUSICLIB_API std::uint32_t tryScaleSignedNormalizedDoubleto32(double signedNormalizedValue);

    /// Scale a 32-bit value ([0,.. 0x80000000,.. 0xFFFFFFFF]) to a signed normalized value ([-1.0,.. 0.0,.. 1.0]).
    MUSICLIB_API double scale32toSignedNormalizedDouble(std::uint32_t highResValue);

} // namespace bw_music

#include <MusicLib/Utilities/valueResolutionConversion_inl.hpp>