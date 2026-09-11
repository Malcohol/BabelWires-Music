/**
 * Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <cassert>

#include <BaseLib/Hash/hash.hpp>
#include <BaseLib/Result/error.hpp>

#include <algorithm>
#include <limits>

template <std::uint8_t numSourceBits, std::uint8_t numDestBits, bw_music::UInt64Compatible DestInt>
constexpr DestInt bw_music::detail::uintScale(std::uint64_t sourceValue) {
    static_assert(numSourceBits > 0, "numSourceBits must be greater than 0");
    static_assert(numSourceBits <= 64, "numSourceBits must be less than or equal to 64");
    static_assert(numDestBits > 0, "numDestBits must be greater than 0");
    static_assert(numDestBits <= 64, "numDestBits must be less than or equal to 64");
    if constexpr (numSourceBits < 64) {
        assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");
    }

    if constexpr (numSourceBits == numDestBits) {
        return static_cast<DestInt>(sourceValue);
    } else if constexpr (numSourceBits < numDestBits) {
        const std::uint8_t numScaleBits = (numDestBits - numSourceBits);
        std::uint64_t result = sourceValue << numScaleBits;

        std::uint64_t bitsToRepeat = sourceValue;
        if (numScaleBits > numSourceBits) {
            bitsToRepeat <<= (numScaleBits - numSourceBits);
        } else {
            bitsToRepeat >>= (numSourceBits - numScaleBits);
        }
        while (bitsToRepeat != 0) {
            result |= bitsToRepeat;
            bitsToRepeat >>= numSourceBits;
        }
        return static_cast<DestInt>(result);
    } else {
        const std::uint8_t numScaleBits = (numSourceBits - numDestBits);
        return static_cast<DestInt>(sourceValue >> numScaleBits);
    }
}

template <bw_music::UInt64Compatible SourceInt, bw_music::UInt64Compatible DestInt>
constexpr DestInt bw_music::detail::uintScale(SourceInt sourceValue) {
    return uintScale<static_cast<std::uint8_t>(sizeof(SourceInt) * 8u),
                     static_cast<std::uint8_t>(sizeof(DestInt) * 8u), DestInt>(static_cast<std::uint64_t>(sourceValue));
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
constexpr bw_music::MinMaxValueT<STORAGE_TYPE>::MinMaxValueT(STORAGE_TYPE value)
    : m_value(value) {}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits, bw_music::UInt64Compatible UnsignedInt>
babelwires::ResultT<bw_music::MinMaxValueT<STORAGE_TYPE>> bw_music::MinMaxValueT<STORAGE_TYPE>::fromUnsigned(UnsignedInt value) {
    if constexpr (numSourceBits < 64) {
        if (value >= (static_cast<std::uint64_t>(1) << numSourceBits)) {
            return babelwires::Error() << "Value " << value << " is out of range for the specified number of source bits "
                                       << static_cast<std::uint32_t>(numSourceBits);
        }
    }
    return MinMaxValueT(bw_music::detail::uintScale<numSourceBits, c_storageBits, STORAGE_TYPE>(static_cast<std::uint64_t>(value)));
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits, bw_music::UInt64Compatible UnsignedInt>
constexpr bw_music::MinMaxValueT<STORAGE_TYPE> bw_music::MinMaxValueT<STORAGE_TYPE>::tryFromUnsigned(UnsignedInt value) {
    std::uint64_t clampedValue = value;
    if constexpr (numSourceBits < 64) {
        const std::uint64_t maxValue = (static_cast<std::uint64_t>(1) << numSourceBits) - 1;
        clampedValue = std::min(clampedValue, maxValue);
    }
    return MinMaxValueT(bw_music::detail::uintScale<numSourceBits, c_storageBits, STORAGE_TYPE>(clampedValue));
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits, bw_music::UInt64Compatible UnsignedInt>
constexpr bw_music::MinMaxValueT<STORAGE_TYPE> bw_music::MinMaxValueT<STORAGE_TYPE>::assertFromUnsigned(UnsignedInt value) {
    return MinMaxValueT(bw_music::detail::uintScale<numSourceBits, c_storageBits, STORAGE_TYPE>(static_cast<std::uint64_t>(value)));
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
constexpr std::uint8_t bw_music::MinMaxValueT<STORAGE_TYPE>::getUnsigned8() const {
    return bw_music::detail::uintScale<STORAGE_TYPE, std::uint8_t>(m_value);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
constexpr std::uint16_t bw_music::MinMaxValueT<STORAGE_TYPE>::getUnsigned16() const {
    return bw_music::detail::uintScale<STORAGE_TYPE, std::uint16_t>(m_value);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
constexpr std::uint32_t bw_music::MinMaxValueT<STORAGE_TYPE>::getUnsigned32() const {
    return bw_music::detail::uintScale<STORAGE_TYPE, std::uint32_t>(m_value);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
constexpr std::uint64_t bw_music::MinMaxValueT<STORAGE_TYPE>::getUnsigned64() const {
    return bw_music::detail::uintScale<STORAGE_TYPE, std::uint64_t>(m_value);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits>
constexpr std::uint64_t bw_music::MinMaxValueT<STORAGE_TYPE>::getUnsigned() const {
    return bw_music::detail::uintScale<c_storageBits, numSourceBits, std::uint64_t>(m_value);
}

constexpr bw_music::MinMaxValue8 bw_music::operator""_mmv8(unsigned long long value) {
    return MinMaxValue8::assertFromUnsigned<8>(value);
}

constexpr bw_music::MinMaxValue16 bw_music::operator""_mmv16(unsigned long long value) {
    return MinMaxValue16::assertFromUnsigned<16>(value);
}

constexpr bw_music::MinMaxValue32 bw_music::operator""_mmv32(unsigned long long value) {
    return MinMaxValue32::assertFromUnsigned<32>(value);
}

constexpr bw_music::MinMaxValue64 bw_music::operator""_mmv64(unsigned long long value) {
    return MinMaxValue64::assertFromUnsigned<64>(value);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
inline bw_music::MinMaxValueT<STORAGE_TYPE> bw_music::MinMaxValueT<STORAGE_TYPE>::assertFromNormalizedDouble(
    double normalizedValue) {
    assert((normalizedValue >= 0.0) && (normalizedValue <= 1.0) && "Normalized value must be in [0.0, 1.0]");
    return MinMaxValueT(static_cast<STORAGE_TYPE>(std::numeric_limits<STORAGE_TYPE>::max() * normalizedValue));
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
babelwires::ResultT<bw_music::MinMaxValueT<STORAGE_TYPE>> bw_music::MinMaxValueT<STORAGE_TYPE>::fromNormalizedDouble(
    double normalizedValue) {
    if ((normalizedValue < 0.0) || (normalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized value must be in [0.0, 1.0]";
    }
    return assertFromNormalizedDouble(normalizedValue);
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
bw_music::MinMaxValueT<STORAGE_TYPE> bw_music::MinMaxValueT<STORAGE_TYPE>::tryFromNormalizedDouble(double normalizedValue) {
    if (normalizedValue < 0.0) {
        return MinMaxValueT(0u);
    } else if (normalizedValue > 1.0) {
        return MinMaxValueT(std::numeric_limits<STORAGE_TYPE>::max());
    } else {
        return assertFromNormalizedDouble(normalizedValue);
    }
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
double bw_music::MinMaxValueT<STORAGE_TYPE>::getNormalizedDouble() const {
    return static_cast<double>(m_value) / std::numeric_limits<STORAGE_TYPE>::max();
}

template <bw_music::MinMaxValueStorageType STORAGE_TYPE>
std::size_t bw_music::MinMaxValueT<STORAGE_TYPE>::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NNI"), m_value);
}