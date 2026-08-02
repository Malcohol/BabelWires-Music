/**
 * A representation of an integer value in the range [0, 0xFFFFFFFF].
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
    concept UnsignedInt32Compatible = std::unsigned_integral<T> && !std::same_as<std::remove_cvref_t<T>, bool> &&
                                      (sizeof(std::remove_cvref_t<T>) <= sizeof(std::uint32_t));

    /// Represents a value in the range [0, 0xFFFFFFFF].
    struct MUSICLIB_API MinMaxValue {
        /// Construct from a value in the range [0, 0xFFFFFFFF].
        static MinMaxValue fromUnsigned32(std::uint32_t highResValue);
        template <UnsignedInt32Compatible UnsignedInt> static MinMaxValue fromUnsigned32(UnsignedInt highResValue);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        template <std::uint8_t numSourceBits>
        static babelwires::ResultT<MinMaxValue> fromUnsigned(std::uint32_t value);
        template <std::uint8_t numSourceBits, UnsignedInt32Compatible UnsignedInt>
        static babelwires::ResultT<MinMaxValue> fromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Clamps to the maximum value if the input is out of range.
        template <std::uint8_t numSourceBits> static MinMaxValue tryFromUnsigned(std::uint32_t value);
        template <std::uint8_t numSourceBits, UnsignedInt32Compatible UnsignedInt>
        static MinMaxValue tryFromUnsigned(UnsignedInt value);

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits> static MinMaxValue assertFromUnsigned(std::uint32_t value);
        template <std::uint8_t numSourceBits, UnsignedInt32Compatible UnsignedInt>
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

        /// Get a value in the range [0, 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Construct from a value in the range [0, (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint32_t getUnsigned() const;

        /// Get a value in the range [0, 1.0].
        double getNormalizedDouble() const;

        std::size_t getHash() const;

        auto operator<=>(const MinMaxValue&) const = default;

      public:
        template <std::signed_integral SignedInt> static MinMaxValue fromUnsigned32(SignedInt highResValue) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static babelwires::ResultT<MinMaxValue> fromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValue tryFromUnsigned(SignedInt value) = delete;
        template <std::uint8_t numSourceBits, std::signed_integral SignedInt>
        static MinMaxValue assertFromUnsigned(SignedInt value) = delete;

      protected:
        /// Construct from a value in the range [0, 0xFFFFFFFF].
        MinMaxValue(std::uint32_t highResValue);

      private:
        /// The value in the range [0, 0xFFFFFFFF].
        std::uint32_t m_value;
    };

    namespace detail {
        /// Scale the source value from a smaller to a larger domain of values.
        /// Assumes that the domain of source values is the range [0, (2^numSourceBits) - 1]
        /// and that the domain of dest values is the range [0, (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
        constexpr std::uint32_t uintScaleUp(std::uint32_t sourceValue);

        /// Scale the source value from a larger to a smaller domain of values.
        /// Assumes that the domain of source values is the range [0, (2^numSourceBits) - 1]
        /// and that the domain of dest values is the range [0, (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
        constexpr std::uint32_t uintScaleDown(std::uint32_t sourceValue);

    } // namespace detail
} // namespace bw_music

namespace std {
    template <> struct hash<bw_music::MinMaxValue> {
        inline std::size_t operator()(const bw_music::MinMaxValue& aci) const { return aci.getHash(); }
    };
} // namespace std

#include <MusicLib/Utilities/minMaxValue_inl.hpp>
