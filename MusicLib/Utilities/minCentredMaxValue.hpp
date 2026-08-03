/**
 * Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/Result/result.hpp>

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace bw_music {
    template <typename T>
    concept UInt64Compatible = std::unsigned_integral<T> && !std::same_as<std::remove_cvref_t<T>, bool> &&
                               (sizeof(std::remove_cvref_t<T>) <= sizeof(std::uint64_t));

    template <typename T>
    concept MinCentredMaxValueStorageType =
        std::same_as<std::remove_cvref_t<T>, std::uint8_t> || std::same_as<std::remove_cvref_t<T>, std::uint16_t> ||
        std::same_as<std::remove_cvref_t<T>, std::uint32_t> || std::same_as<std::remove_cvref_t<T>, std::uint64_t>;

    /// Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
    /// Semantically the lowest and highest values should be considered the same distance from the centre value, even
    /// though the lower half of the range can represent one additional intermediate value.
    /// Calculations should not use this representation but should use the signed normalized double methods.
    /// This representation is used by some MIDI data, such as Pan or Pitch Bend.
    /// Four size-specific specializations are provided: MinCentredMaxValue8, MinCentredMaxValue16,
    /// MinCentredMaxValue32, and MinCentredMaxValue64.
    template <MinCentredMaxValueStorageType STORAGE_TYPE> class MUSICLIB_API MinCentredMaxValueT {
      public:
        using StorageType = STORAGE_TYPE;

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        /// Note: There's no best effort "try" equivalent to this method: If the value is out of range, this class
        /// doesn't have the context to repair the situation.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static babelwires::ResultT<MinCentredMaxValueT> fromUnsigned(UnsignedInt value);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static MinCentredMaxValueT assertFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Fails if the value is out of range.
        static babelwires::ResultT<MinCentredMaxValueT> fromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Clamps to the nearest valid value if the input is out of range.
        static MinCentredMaxValueT tryFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        static MinCentredMaxValueT assertFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Get a value in the asymmetric range [0,... 0x80,.. 0xFF].
        std::uint8_t getUnsigned8() const;

        /// Get a value in the asymmetric range [0,... 0x8000,.. 0xFFFF].
        std::uint16_t getUnsigned16() const;

        /// Get a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Get a value in the asymmetric range [0,... 2^63,.. 0xFFFFFFFFFFFFFFFF].
        std::uint64_t getUnsigned64() const;

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint64_t getUnsigned() const;

        /// Get a value in the range [-1.0,.. 0.0,.. 1.0].
        double getSignedNormalizedValue() const;

        std::size_t getHash() const;

        auto operator<=>(const MinCentredMaxValueT&) const = default;

      public:
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static babelwires::ResultT<MinCentredMaxValueT> fromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinCentredMaxValueT assertFromUnsigned(SignedInt value) = delete;

      protected:
        /// Construct from a value in the storage type's full asymmetric min-centre-max range.
        MinCentredMaxValueT(STORAGE_TYPE value);

      private:
        static constexpr std::uint8_t c_storageBits = static_cast<std::uint8_t>(sizeof(STORAGE_TYPE) * 8u);

        /// The value scaled into the storage type's full asymmetric min-centre-max range.
        STORAGE_TYPE m_value;
    };

    using MinCentredMaxValue8 = MinCentredMaxValueT<std::uint8_t>;
    using MinCentredMaxValue16 = MinCentredMaxValueT<std::uint16_t>;
    using MinCentredMaxValue32 = MinCentredMaxValueT<std::uint32_t>;
    using MinCentredMaxValue64 = MinCentredMaxValueT<std::uint64_t>;

    namespace detail {
        /// Scale the source value between asymmetric min-centre-max unsigned domains, including upscaling,
        /// downscaling, and no-op cases.
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits, UInt64Compatible DestInt>
        constexpr DestInt minCentreMaxScale(std::uint64_t sourceValue);

        /// Scale the source value between full-width asymmetric min-centre-max unsigned integer types.
        template <UInt64Compatible SourceInt, UInt64Compatible DestInt>
        constexpr DestInt minCentreMaxScale(SourceInt sourceValue);

    } // namespace detail
} // namespace bw_music

namespace std {
    template <bw_music::MinCentredMaxValueStorageType STORAGE_TYPE>
    struct hash<bw_music::MinCentredMaxValueT<STORAGE_TYPE>> {
        inline std::size_t operator()(const bw_music::MinCentredMaxValueT<STORAGE_TYPE>& aci) const {
            return aci.getHash();
        }
    };
} // namespace std

#include <MusicLib/Utilities/minCentredMaxValue_inl.hpp>