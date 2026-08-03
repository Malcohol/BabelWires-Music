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
    concept UInt64Compatible = std::unsigned_integral<T> && !std::same_as<std::remove_cvref_t<T>, bool> &&
                               (sizeof(std::remove_cvref_t<T>) <= sizeof(std::uint64_t));

    template <typename T>
    concept MinMaxValueStorageType =
        std::same_as<std::remove_cvref_t<T>, std::uint8_t> || std::same_as<std::remove_cvref_t<T>, std::uint16_t> ||
        std::same_as<std::remove_cvref_t<T>, std::uint32_t> || std::same_as<std::remove_cvref_t<T>, std::uint64_t>;

    /// Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
    /// The API is strict by design, to avoid situations where C++ type conversion would be ambiguous or
    /// incorrect.
    /// Four size-specific specializations are provided: MinMaxValue8, MinMaxValue16, MinMaxValue32, and MinMaxValue64.
    template <MinMaxValueStorageType STORAGE_TYPE> class MUSICLIB_API MinMaxValueT {
      public:
        using StorageType = STORAGE_TYPE;

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static babelwires::ResultT<MinMaxValueT> fromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Clamps to the maximum value if the input is out of range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static MinMaxValueT tryFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits, UInt64Compatible UnsignedInt>
        static MinMaxValueT assertFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, 1.0].
        /// Fails if the value is out of range.
        static babelwires::ResultT<MinMaxValueT> fromNormalizedDouble(double normalizedValue);

        /// Construct from a value in the range [0, 1.0].
        /// Clamps to the nearest valid value if the input is out of range.
        static MinMaxValueT tryFromNormalizedDouble(double normalizedValue);

        /// Construct from a value in the range [0, 1.0].
        /// Asserts that the value is in range.
        static MinMaxValueT assertFromNormalizedDouble(double normalizedValue);

        /// Get a value in the range [0, 0xFF].
        std::uint8_t getUnsigned8() const;

        /// Get a value in the range [0, 0xFFFF].
        std::uint16_t getUnsigned16() const;

        /// Get a value in the range [0, 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Get a value in the range [0, 0xFFFFFFFFFFFFFFFF].
        std::uint64_t getUnsigned64() const;

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint64_t getUnsigned() const;

        /// Get a value in the range [0, 1.0].
        double getNormalizedDouble() const;

        std::size_t getHash() const;

        auto operator<=>(const MinMaxValueT&) const = default;

      public:
        // Delete many alternatives to avoid situations where C++ type conversion would be ambiguous or incorrect.
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static babelwires::ResultT<MinMaxValueT> fromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValueT tryFromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValueT assertFromUnsigned(SignedInt value) = delete;

      protected:
        /// Construct from a value in the storage type's full unsigned range.
        MinMaxValueT(STORAGE_TYPE value);

      private:
        static constexpr std::uint8_t c_storageBits = static_cast<std::uint8_t>(sizeof(STORAGE_TYPE) * 8u);

        /// The value scaled into the storage type's full unsigned range.
        STORAGE_TYPE m_value;
    };

    using MinMaxValue8 = MinMaxValueT<std::uint8_t>;
    using MinMaxValue16 = MinMaxValueT<std::uint16_t>;
    using MinMaxValue32 = MinMaxValueT<std::uint32_t>;
    using MinMaxValue64 = MinMaxValueT<std::uint64_t>;

    namespace detail {
        /// Scale the source value between unsigned domains, including upscaling, downscaling, and no-op cases.
        /// Assumes that the source value is in the range [0, (2^numSourceBits) - 1]
        /// and that the destination domain is [0, (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits, UInt64Compatible DestInt>
        constexpr DestInt uintScale(std::uint64_t sourceValue);

        /// Scale the source value between full-width unsigned integer types.
        template <UInt64Compatible SourceInt, UInt64Compatible DestInt>
        constexpr DestInt uintScale(SourceInt sourceValue);

    } // namespace detail
} // namespace bw_music

namespace std {
    template <bw_music::MinMaxValueStorageType STORAGE_TYPE> struct hash<bw_music::MinMaxValueT<STORAGE_TYPE>> {
        inline std::size_t operator()(const bw_music::MinMaxValueT<STORAGE_TYPE>& aci) const { return aci.getHash(); }
    };
} // namespace std

#include <MusicLib/Utilities/minMaxValue_inl.hpp>
