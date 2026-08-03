/**
 * Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <cassert>

#include <BaseLib/Hash/hash.hpp>
#include <BaseLib/Result/error.hpp>

#include <limits>

// See M2-115-U MIDI 2.0 Bit Scaling and Resolution Conversion.
template <std::uint8_t numSourceBits, std::uint8_t numDestBits, bw_music::UInt64Compatible DestInt>
constexpr DestInt bw_music::detail::minCentreMaxScale(std::uint64_t sourceValue) {
    static_assert(numSourceBits > 1, "numSourceBits must be greater than 1");
    static_assert(numSourceBits <= 64, "numSourceBits must be less than or equal to 64");
    static_assert(numDestBits > 1, "numDestBits must be greater than 1");
    static_assert(numDestBits <= 64, "numDestBits must be less than or equal to 64");
    if constexpr (numSourceBits < 64) {
        assert((sourceValue < (static_cast<std::uint64_t>(1) << numSourceBits)) && "sourceValue is out of range");
    }

    if constexpr (numSourceBits == numDestBits) {
        return static_cast<DestInt>(sourceValue);
    } else if constexpr (numSourceBits < numDestBits) {
        const std::uint8_t numScaleBits = (numDestBits - numSourceBits);
        const std::uint64_t srcCentre = (static_cast<std::uint64_t>(1) << (numSourceBits - 1));
        std::uint64_t result = sourceValue << numScaleBits;
        if (sourceValue <= srcCentre) {
            return static_cast<DestInt>(result);
        }

        const std::uint8_t numRepeatBits = numSourceBits - 1;
        std::uint64_t bitsToRepeat = sourceValue & ((static_cast<std::uint64_t>(1) << numRepeatBits) - 1);
        if (numScaleBits > numRepeatBits) {
            bitsToRepeat <<= (numScaleBits - numRepeatBits);
        } else {
            bitsToRepeat >>= (numRepeatBits - numScaleBits);
        }
        while (bitsToRepeat != 0) {
            result |= bitsToRepeat;
            bitsToRepeat >>= numRepeatBits;
        }
        return static_cast<DestInt>(result);
    } else {
        const std::uint8_t numScaleBits = (numSourceBits - numDestBits);
        return static_cast<DestInt>(sourceValue >> numScaleBits);
    }
}

template <bw_music::UInt64Compatible SourceInt, bw_music::UInt64Compatible DestInt>
constexpr DestInt bw_music::detail::minCentreMaxScale(SourceInt sourceValue) {
    return minCentreMaxScale<static_cast<std::uint8_t>(sizeof(SourceInt) * 8u),
                             static_cast<std::uint8_t>(sizeof(DestInt) * 8u), DestInt>(
        static_cast<std::uint64_t>(sourceValue));
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline bw_music::MinCentredMaxValueT<STORAGE_TYPE>::MinCentredMaxValueT(STORAGE_TYPE value)
    : m_value(value) {}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits, bw_music::UInt64Compatible UnsignedInt>
babelwires::ResultT<bw_music::MinCentredMaxValueT<STORAGE_TYPE>>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>::fromUnsigned(UnsignedInt value) {
    static_assert(numSourceBits <= 64, "numSourceBits must be less than or equal to 64");
    if constexpr (numSourceBits < 64) {
        if (value >= (static_cast<std::uint64_t>(1) << numSourceBits)) {
            return babelwires::Error() << "Value is out of range for the specified number of source bits";
        }
    }
    return MinCentredMaxValueT(
        bw_music::detail::minCentreMaxScale<numSourceBits, c_storageBits, STORAGE_TYPE>(static_cast<std::uint64_t>(value)));
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits, bw_music::UInt64Compatible UnsignedInt>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>::assertFromUnsigned(UnsignedInt value) {
    static_assert(numSourceBits <= 64, "numSourceBits must be less than or equal to 64");
    return MinCentredMaxValueT(
        bw_music::detail::minCentreMaxScale<numSourceBits, c_storageBits, STORAGE_TYPE>(static_cast<std::uint64_t>(value)));
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline std::uint8_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getUnsigned8() const {
    return bw_music::detail::minCentreMaxScale<STORAGE_TYPE, std::uint8_t>(m_value);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline std::uint16_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getUnsigned16() const {
    return bw_music::detail::minCentreMaxScale<STORAGE_TYPE, std::uint16_t>(m_value);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline std::uint32_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getUnsigned32() const {
    return bw_music::detail::minCentreMaxScale<STORAGE_TYPE, std::uint32_t>(m_value);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline std::uint64_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getUnsigned64() const {
    return bw_music::detail::minCentreMaxScale<STORAGE_TYPE, std::uint64_t>(m_value);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
template <std::uint8_t numSourceBits>
std::uint64_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getUnsigned() const {
    static_assert(numSourceBits <= 64, "numSourceBits must be less than or equal to 64");
    return bw_music::detail::minCentreMaxScale<c_storageBits, numSourceBits, std::uint64_t>(m_value);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
inline bw_music::MinCentredMaxValueT<STORAGE_TYPE>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>::assertFromSignedNormalizedDouble(double signedNormalizedValue) {
    assert((signedNormalizedValue >= -1.0) && (signedNormalizedValue <= 1.0) &&
           "Normalized value must be in [-1.0, 1.0]");
    constexpr std::uint64_t centreValue = (static_cast<std::uint64_t>(1) << (c_storageBits - 1));
    constexpr long double centre = static_cast<long double>(centreValue);

    if (signedNormalizedValue < 0.0) {
        return MinCentredMaxValueT(static_cast<STORAGE_TYPE>(centre * (1.0L + signedNormalizedValue)));
    } else {
        return MinCentredMaxValueT(static_cast<STORAGE_TYPE>(centre + ((centre - 1.0L) * signedNormalizedValue)));
    }
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
babelwires::ResultT<bw_music::MinCentredMaxValueT<STORAGE_TYPE>>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>::fromSignedNormalizedDouble(double signedNormalizedValue) {
    if ((signedNormalizedValue < -1.0) || (signedNormalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized signed value must be in [-1.0, 1.0]";
    }
    return assertFromSignedNormalizedDouble(signedNormalizedValue);
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>
bw_music::MinCentredMaxValueT<STORAGE_TYPE>::tryFromSignedNormalizedDouble(double signedNormalizedValue) {
    if (signedNormalizedValue < -1.0) {
        return assertFromUnsigned<32>(0u);
    } else if (signedNormalizedValue > 1.0) {
        return assertFromUnsigned<32>(std::numeric_limits<std::uint32_t>::max());
    } else {
        return assertFromSignedNormalizedDouble(signedNormalizedValue);
    }
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
double bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getSignedNormalizedValue() const {
    constexpr std::uint64_t centreValue = (static_cast<std::uint64_t>(1) << (c_storageBits - 1));
    constexpr long double centre = static_cast<long double>(centreValue);

    if (m_value < centreValue) {
        return static_cast<double>((static_cast<long double>(m_value) / centre) - 1.0L);
    } else {
        return static_cast<double>(static_cast<long double>(m_value - centreValue) / (centre - 1.0L));
    }
}

template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
std::size_t bw_music::MinCentredMaxValueT<STORAGE_TYPE>::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ACI"), m_value);
}
