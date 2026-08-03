/**
 * Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
 *
 * (C) 2021 Malcolm Tyrrell
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
    concept UInt32Compatible = std::unsigned_integral<T> && !std::same_as<std::remove_cvref_t<T>, bool> &&
                                      (sizeof(std::remove_cvref_t<T>) <= sizeof(std::uint32_t));

    template <typename T>
    concept MinMaxValueStorageType =
        std::same_as<std::remove_cvref_t<T>, std::uint8_t> || std::same_as<std::remove_cvref_t<T>, std::uint16_t> ||
        std::same_as<std::remove_cvref_t<T>, std::uint32_t>;

    /// Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
    /// The API is strict by design, to avoid situations where C++ type conversion would be ambiguous or
    /// incorrect.
    template <MinMaxValueStorageType STORAGE_TYPE = std::uint32_t> class MUSICLIB_API MinMaxValue {
      public:
        using StorageType = STORAGE_TYPE;

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        template <std::uint8_t numSourceBits, UInt32Compatible UnsignedInt>
        static babelwires::ResultT<MinMaxValue> fromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Clamps to the maximum value if the input is out of range.
        template <std::uint8_t numSourceBits, UInt32Compatible UnsignedInt>
        static MinMaxValue tryFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits, UInt32Compatible UnsignedInt>
        static MinMaxValue assertFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, 1.0].
        /// Fails if the value is out of range.
        static babelwires::ResultT<MinMaxValue> fromNormalizedDouble(double normalizedValue);

        /// Construct from a value in the range [0, 1.0].
        /// Clamps to the nearest valid value if the input is out of range.
        static MinMaxValue tryFromNormalizedDouble(double normalizedValue);

        /// Construct from a value in the range [0, 1.0].
        /// Asserts that the value is in range.
        static MinMaxValue assertFromNormalizedDouble(double normalizedValue);

        /// Get a value in the range [0, 0xFF].
        std::uint8_t getUnsigned8() const;

        /// Get a value in the range [0, 0xFFFF].
        std::uint16_t getUnsigned16() const;

        /// Get a value in the range [0, 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint32_t getUnsigned() const;

        /// Get a value in the range [0, 1.0].
        double getNormalizedDouble() const;

        std::size_t getHash() const;

        auto operator<=>(const MinMaxValue&) const = default;

      public:
        // Delete many alternatives to avoid situations where C++ type conversion would be ambiguous or incorrect.
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static babelwires::ResultT<MinMaxValue> fromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValue tryFromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValue assertFromUnsigned(SignedInt value) = delete;

      protected:
        /// Construct from a value in the storage type's full unsigned range.
        MinMaxValue(STORAGE_TYPE value);

      private:
        static constexpr std::uint8_t c_storageBits = static_cast<std::uint8_t>(sizeof(STORAGE_TYPE) * 8u);

        /// The value scaled into the storage type's full unsigned range.
        STORAGE_TYPE m_value;
    };

    namespace detail {
        /// Scale the source value between unsigned domains, including upscaling, downscaling, and no-op cases.
        /// Assumes that the source value is in the range [0, (2^numSourceBits) - 1]
        /// and that the destination domain is [0, (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits, UInt32Compatible DestInt = std::uint32_t>
        constexpr DestInt uintScale(std::uint32_t sourceValue);

        /// Scale the source value between full-width unsigned integer types.
        template <UInt32Compatible SourceInt, UInt32Compatible DestInt>
        constexpr DestInt uintScale(SourceInt sourceValue);

    } // namespace detail
} // namespace bw_music

namespace std {
    template <bw_music::MinMaxValueStorageType STORAGE_TYPE> struct hash<bw_music::MinMaxValue<STORAGE_TYPE>> {
        inline std::size_t operator()(const bw_music::MinMaxValue<STORAGE_TYPE>& aci) const { return aci.getHash(); }
    };
} // namespace std

#include <MusicLib/Utilities/minMaxValue_inl.hpp>
