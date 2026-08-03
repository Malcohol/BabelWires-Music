/**
 * Represents a value between a minimum and maximum, with a distinguished centred value, using an integer.
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
    concept MinCentreMaxValueStorageType =
        std::same_as<std::remove_cvref_t<T>, std::uint8_t> || std::same_as<std::remove_cvref_t<T>, std::uint16_t> ||
        std::same_as<std::remove_cvref_t<T>, std::uint32_t> || std::same_as<std::remove_cvref_t<T>, std::uint64_t>;

    /// Represents a value between a minimum and maximum, with a distinguished centred value, using an integer distributed across the range
    /// in a slightly asymmetric manner. 
    /// Semantically the lowest and highest values should be considered the same distance from the centre value, even
    /// though the lower half of the range can represent one additional intermediate value.
    /// The API is strict by design, to avoid situations where C++ type conversion would be ambiguous or incorrect.
    /// Calculations should not use this representation but should use the signed normalized double methods.
    /// This representation is used by some MIDI data, such as Pan or Pitch Bend.
    /// Four size-specific specializations are provided: MinCentreMaxValue8, MinCentreMaxValue16,
    /// MinCentreMaxValue32, and MinCentreMaxValue64.
    template <MinCentreMaxValueStorageType STORAGE_TYPE> class MUSICLIB_API MinCentreMaxValueT {
      public:
        using StorageType = STORAGE_TYPE;

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static babelwires::ResultT<MinCentreMaxValueT> fromUnsigned(UnsignedInt value);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Clamps to the nearest valid value if the input is out of range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static MinCentreMaxValueT tryFromUnsigned(UnsignedInt value);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static MinCentreMaxValueT assertFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Fails if the value is out of range.
        static babelwires::ResultT<MinCentreMaxValueT> fromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Clamps to the nearest valid value if the input is out of range.
        static MinCentreMaxValueT tryFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        static MinCentreMaxValueT assertFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint64_t getUnsigned() const;

        /// Get a value in the range [-1.0,.. 0.0,.. 1.0].
        double getSignedNormalizedValue() const;

        std::size_t getHash() const;

        auto operator<=>(const MinCentreMaxValueT&) const = default;

        // Convenience methods

        /// Get a value in the asymmetric range [0,... 0x80,.. 0xFF].
        std::uint8_t getUnsigned8() const;

        /// Get a value in the asymmetric range [0,... 0x8000,.. 0xFFFF].
        std::uint16_t getUnsigned16() const;

        /// Get a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Get a value in the asymmetric range [0,... 0x8000000000000000,.. 0xFFFFFFFFFFFFFFFF].
        std::uint64_t getUnsigned64() const;

      public:
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static babelwires::ResultT<MinCentreMaxValueT> fromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinCentreMaxValueT assertFromUnsigned(SignedInt value) = delete;

      protected:
        /// Construct from a value in the storage type's full asymmetric min-centre-max range.
        MinCentreMaxValueT(STORAGE_TYPE value);

      private:
        static constexpr std::uint8_t c_storageBits = static_cast<std::uint8_t>(sizeof(STORAGE_TYPE) * 8u);

        /// The value scaled into the storage type's full asymmetric min-centre-max range.
        STORAGE_TYPE m_value;
    };

    using MinCentreMaxValue8 = MinCentreMaxValueT<std::uint8_t>;
    using MinCentreMaxValue16 = MinCentreMaxValueT<std::uint16_t>;
    using MinCentreMaxValue32 = MinCentreMaxValueT<std::uint32_t>;
    using MinCentreMaxValue64 = MinCentreMaxValueT<std::uint64_t>;

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
    template <bw_music::MinCentreMaxValueStorageType STORAGE_TYPE>
    struct hash<bw_music::MinCentreMaxValueT<STORAGE_TYPE>> {
        inline std::size_t operator()(const bw_music::MinCentreMaxValueT<STORAGE_TYPE>& aci) const {
            return aci.getHash();
        }
    };
} // namespace std

#include <MusicLib/Utilities/minCentreMaxValue_inl.hpp>