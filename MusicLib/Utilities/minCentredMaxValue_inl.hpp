/**
 * Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <cassert>
#include <BaseLib/Result/error.hpp>

// See M2-115-U MIDI 2.0 Bit Scaling and Resolution Conversion.
template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
constexpr std::uint32_t bw_music::detail::minCentreMaxScaleUp(std::uint32_t sourceValue) {
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
constexpr std::uint32_t bw_music::detail::minCentreMaxScaleDown(std::uint32_t sourceValue) {
    static_assert(numDestBits > 1, "numDestBits must be greater than 1");
    static_assert(numSourceBits <= 32, "numSourceBits must be less than or equal to 32");
    static_assert(numSourceBits > numDestBits, "numSourceBits must be greater than numDestBits");
    assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");

    const std::uint8_t numScaleBits = (numSourceBits - numDestBits);
    return sourceValue >> numScaleBits;
}

inline bw_music::MinCentredMaxValue::MinCentredMaxValue(std::uint32_t highResValue)
    : m_value(highResValue) {}

inline bw_music::MinCentredMaxValue bw_music::MinCentredMaxValue::fromUnsigned32(std::uint32_t highResValue) {
    return MinCentredMaxValue(highResValue);
}

template <std::uint8_t numSourceBits>
babelwires::ResultT<bw_music::MinCentredMaxValue> bw_music::MinCentredMaxValue::fromUnsigned(std::uint32_t value) {
    if (value >= (1 << numSourceBits)) {
        return babelwires::Error() << "Value is out of range for the specified number of source bits";
    }
    return MinCentredMaxValue(bw_music::detail::minCentreMaxScaleUp<numSourceBits, 32>(value));
}

template <std::uint8_t numSourceBits>
bw_music::MinCentredMaxValue bw_music::MinCentredMaxValue::assertFromUnsigned(std::uint32_t value) {
    return MinCentredMaxValue(bw_music::detail::minCentreMaxScaleUp<numSourceBits, 32>(value));
}

inline std::uint32_t bw_music::MinCentredMaxValue::getUnsigned32() const {
    return m_value;
}

/// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
template <std::uint8_t numSourceBits> std::uint32_t bw_music::MinCentredMaxValue::getUnsigned() const {
    return bw_music::detail::minCentreMaxScaleDown<32, numSourceBits>(m_value);
}
