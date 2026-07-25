/**
 * Some utility functions useful for scaling between values of different resolutions, such as MIDI 1.0 and MIDI 2.0.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <cassert>

// See M2-115-U MIDI 2.0 Bit Scaling and Resolution Conversion.
template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
constexpr std::uint32_t bw_music::minCentreMaxScaleUp(std::uint32_t sourceValue) {
    static_assert(numSourceBits > 1, "numSourceBits must be greater than 1");
    static_assert(numDestBits <= 32, "numDestBits must be less than or equal to 32");
    static_assert(numSourceBits < numDestBits, "numSourceBits must be less than numDestBits");
    assert((sourceValue < (1 << numSourceBits)) && "sourceValue is out of range");

    const std::uint8_t numScaleBits = (numDestBits - numSourceBits);
    const std::uint32_t srcCentre = 1 << (numSourceBits - 1);
    std::uint32_t result = sourceValue << numScaleBits;
    if (sourceValue <= srcCentre) {
        return result;
    }

    const std::uint8_t numRepeatBits = numSourceBits - 1;
    std::uint32_t bitsToRepeat = sourceValue & ((1 << numRepeatBits) - 1);
    if (numScaleBits > numRepeatBits) {
        bitsToRepeat <<= (numScaleBits - numRepeatBits);
    } else {
        bitsToRepeat >>= (numRepeatBits - numScaleBits);
    }
    while (bitsToRepeat != 0) {
        result |= bitsToRepeat;
        bitsToRepeat >>= numRepeatBits;
    }
    return result;
}

template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
constexpr std::uint32_t bw_music::minCentreMaxScaleDown(std::uint32_t sourceValue) {
    static_assert(numDestBits > 1, "numDestBits must be greater than 1");
    static_assert(numSourceBits <= 32, "numSourceBits must be less than or equal to 32");
    static_assert(numSourceBits > numDestBits, "numSourceBits must be greater than numDestBits");
    assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");

    const std::uint8_t numScaleBits = (numSourceBits - numDestBits);
    return sourceValue >> numScaleBits;
}
