/**
 * Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <cassert>

#include <BaseLib/Result/error.hpp>

template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
constexpr std::uint32_t bw_music::detail::uintScaleUp(std::uint32_t sourceValue) {
    static_assert(numSourceBits > 0, "numSourceBits must be greater than 0");
    static_assert(numDestBits <= 32, "numDestBits must be less than or equal to 32");
    static_assert(numSourceBits < numDestBits, "numSourceBits must be less than numDestBits");
    assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");

    const std::uint8_t numScaleBits = (numDestBits - numSourceBits);
    std::uint32_t result = sourceValue << numScaleBits;

    std::uint32_t bitsToRepeat = sourceValue;
    if (numScaleBits > numSourceBits) {
        bitsToRepeat <<= (numScaleBits - numSourceBits);
    } else {
        bitsToRepeat >>= (numSourceBits - numScaleBits);
    }
    while (bitsToRepeat != 0) {
        result |= bitsToRepeat;
        bitsToRepeat >>= numSourceBits;
    }
    return result;
}

template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
constexpr std::uint32_t bw_music::detail::uintScaleDown(std::uint32_t sourceValue) {
    static_assert(numDestBits > 0, "numDestBits must be greater than 0");
    static_assert(numSourceBits <= 32, "numSourceBits must be less than or equal to 32");
    static_assert(numSourceBits > numDestBits, "numSourceBits must be greater than numDestBits");
    assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");

    const std::uint8_t numScaleBits = (numSourceBits - numDestBits);
    return sourceValue >> numScaleBits;
}

inline bw_music::MinMaxValue::MinMaxValue(std::uint32_t highResValue)
    : m_value(highResValue) {}

inline bw_music::MinMaxValue bw_music::MinMaxValue::fromUnsigned32(std::uint32_t highResValue) {
    return MinMaxValue(highResValue);
}

template <bw_music::UnsignedInt32Compatible UnsignedInt>
inline bw_music::MinMaxValue bw_music::MinMaxValue::fromUnsigned32(UnsignedInt highResValue) {
    return fromUnsigned32(static_cast<std::uint32_t>(highResValue));
}

template <std::uint8_t numSourceBits>
babelwires::ResultT<bw_music::MinMaxValue> bw_music::MinMaxValue::fromUnsigned(std::uint32_t value) {
    if (value >= (static_cast<std::uint64_t>(1) << numSourceBits)) {
        return babelwires::Error() << "Value is out of range for the specified number of source bits";
    }
    if constexpr (numSourceBits == 32) {
        return MinMaxValue(value);
    } else {
        return MinMaxValue(bw_music::detail::uintScaleUp<numSourceBits, 32>(value));
    }
}

template <std::uint8_t numSourceBits, bw_music::UnsignedInt32Compatible UnsignedInt>
babelwires::ResultT<bw_music::MinMaxValue> bw_music::MinMaxValue::fromUnsigned(UnsignedInt value) {
    return fromUnsigned<numSourceBits>(static_cast<std::uint32_t>(value));
}

template <std::uint8_t numSourceBits>
bw_music::MinMaxValue bw_music::MinMaxValue::tryFromUnsigned(std::uint32_t value) {
    if constexpr (numSourceBits == 32) {
        return MinMaxValue(value);
    } else {
        const std::uint32_t maxValue = static_cast<std::uint32_t>((static_cast<std::uint64_t>(1) << numSourceBits) - 1);
        if (value > maxValue) {
            value = maxValue;
        }
        return MinMaxValue(bw_music::detail::uintScaleUp<numSourceBits, 32>(value));
    }
}

template <std::uint8_t numSourceBits, bw_music::UnsignedInt32Compatible UnsignedInt>
bw_music::MinMaxValue bw_music::MinMaxValue::tryFromUnsigned(UnsignedInt value) {
    return tryFromUnsigned<numSourceBits>(static_cast<std::uint32_t>(value));
}

template <std::uint8_t numSourceBits>
bw_music::MinMaxValue bw_music::MinMaxValue::assertFromUnsigned(std::uint32_t value) {
    if constexpr (numSourceBits == 32) {
        return MinMaxValue(value);
    } else {
        return MinMaxValue(bw_music::detail::uintScaleUp<numSourceBits, 32>(value));
    }
}

template <std::uint8_t numSourceBits, bw_music::UnsignedInt32Compatible UnsignedInt>
bw_music::MinMaxValue bw_music::MinMaxValue::assertFromUnsigned(UnsignedInt value) {
    return assertFromUnsigned<numSourceBits>(static_cast<std::uint32_t>(value));
}

inline std::uint32_t bw_music::MinMaxValue::getUnsigned32() const {
    return m_value;
}

template <std::uint8_t numSourceBits> std::uint32_t bw_music::MinMaxValue::getUnsigned() const {
    if constexpr (numSourceBits == 32) {
        return m_value;
    } else {
        return bw_music::detail::uintScaleDown<32, numSourceBits>(m_value);
    }
}