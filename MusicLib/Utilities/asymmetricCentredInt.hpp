/**
 * Some utility functions useful for scaling between values of different resolutions, such as MIDI 1.0 and MIDI 2.0.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/Result/result.hpp>

#include <cstdint>

namespace bw_music {
    /// Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
    /// Semantically the lowest and highest values should be considered the same distance from the centre value, even
    /// though the lower half of the range can represent one additional intermediate value.
    /// Calculations should not use this stored representation but should use the signed normalized double methods.
    /// This representation is used by some MIDI data, such as Pan or Pitch Bend.
    struct MUSICLIB_API AsymmetricCentredInt {
        /// Construct from a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        static AsymmetricCentredInt fromUnsigned32(std::uint32_t highResValue);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Fails if the value is out of range.
        /// Note: There's no best effort "try" equivalent to this method: If the value is out of range, this class doesn't have the context to repair the situation.
        template <std::uint8_t numSourceBits>
        static babelwires::ResultT<AsymmetricCentredInt> fromUnsigned(std::uint32_t value);

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        /// Asserts that the value is in range.
        template <std::uint8_t numSourceBits>
        static AsymmetricCentredInt assertFromUnsigned(std::uint32_t value);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Fails if the value is out of range.
        static babelwires::ResultT<AsymmetricCentredInt> fromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Clamps to the nearest valid value if the input is out of range.
        static AsymmetricCentredInt tryFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        static AsymmetricCentredInt assertFromSignedNormalizedDouble(double signedNormalizedValue);

        /// Get a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t getUnsigned32() const;

        /// Construct from a value in the asymmetric range [0,... 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template <std::uint8_t numSourceBits> std::uint32_t getUnsigned() const;

        /// Get a value in the range [-1.0,.. 0.0,.. 1.0].
        double getSignedNormalizedValue() const;

        std::size_t getHash() const;

        auto operator<=>(const AsymmetricCentredInt&) const = default;

      protected:
        /// Construct from a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        AsymmetricCentredInt(std::uint32_t highResValue);

      private:
        /// The value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t m_value;
    };

    namespace detail {
        /// Scale the source value from a smaller to a larger domain of values using the MIDI 2.0 "min-centre-max" scaling
        /// scheme.
        /// Assumes that the domain of source values is the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1]
        /// and that the domain of dest values is the range [0,.. 2^(numDestBits - 1),.. (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
        constexpr std::uint32_t minCentreMaxScaleUp(std::uint32_t sourceValue);

        /// Scale the source value from a larger to a smaller domain of values using the MIDI 2.0 "min-centre-max" scaling
        /// scheme.
        /// Assumes that the domain of source values is the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1]
        /// and that the domain of dest values is the range [0,.. 2^(numDestBits - 1),.. (2^numDestBits) - 1].
        template <std::uint8_t numSourceBits, std::uint8_t numDestBits>
        constexpr std::uint32_t minCentreMaxScaleDown(std::uint32_t sourceValue);

    } // namespace detail 
} // namespace bw_music

namespace std {
    template <> struct hash<bw_music::AsymmetricCentredInt> {
        inline std::size_t operator()(const bw_music::AsymmetricCentredInt& aci) const { return aci.getHash(); }
    };
} // namespace std


#include <MusicLib/Utilities/asymmetricCentredInt_inl.hpp>