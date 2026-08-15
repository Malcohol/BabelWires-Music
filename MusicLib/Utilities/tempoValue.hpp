/**
 * A representation of tempo that allows safe round-tripping to and from MIDI tempo storage.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Utilities/tempoValue.hpp>

#include <BaseLib/Result/result.hpp>

namespace bw_music {

    /// A representation of tempo that allows safe round-tripping to and from MIDI tempo storage.
    class MUSICLIB_API TempoValue {
      public:
        TempoValue() = default;

        /// Construct a TempoValue from a tempo in beats per minute.
        /// Returns an error if the value cannot be represented (ignoring loss of precision within the accepted range).
        static babelwires::ResultT<TempoValue> fromBpm(double bpm);

        /// Construct a TempoValue from a tempo in beats per minute.
        /// Clamps the value to the representable range if it cannot be represented (ignoring loss of precision within
        /// the accepted range).
        static TempoValue tryFromBpm(double bpm);

        /// Construct a TempoValue from a tempo in beats per minute.
        /// Asserts that the value can be represented (ignoring loss of precision within the accepted range).
        static TempoValue assertFromBpm(double bpm);

        /// Construct a TempoValue from an integer in the range [0x000001, 0xFFFFFF] representing the number of
        /// microseconds per quarter note. Returns an error if the value is out of range.
        static babelwires::ResultT<TempoValue> fromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Construct a TempoValue from an integer in the range [0x000001, 0xFFFFFF] representing the number of
        /// microseconds per quarter note. Clamps the value if it is out of range.
        static TempoValue tryFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Construct a TempoValue from an integer in the range [0x000001, 0xFFFFFF] representing the number of
        /// microseconds per quarter note. Asserts that the value is in range.
        static TempoValue assertFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Get the tempo in beats per minute.
        double getBpm() const;

        /// Get the contents as an integer in the range [0x000001, 0xFFFFFF] representing the number of microseconds per
        /// quarter note.
        std::uint32_t getMicrosecondsPerQuaternote() const;

        /// The maximum number of decimal places that can be used for rounding BPM values.
        static constexpr int c_maxPrecisionDecimalPlaces = 8;

        /// Get the tempo in beats per minute, rounded to the specified number of decimal places.
        /// Decimal places must be between 0 and c_maxPrecisionDecimalPlaces inclusive.
        /// Note: It is not guaranteed that the rounded value can be converted back to the same TempoValue.
        double getBpmRounded(int decimalPlaces) const;

        /// Get the range of representable BPM values whose endpoints round-trip unchanged at the given precision.
        /// Decimal places must be between 0 and c_maxPrecisionDecimalPlaces inclusive.
        /// Note: While the endpoints of the range are guaranteed to round-trip unchanged, there is no guarantee
        /// that other values within the range will round-trip unchanged.
        /// This is intended for UI use, where the user should be able to select value within the representable range.
        /// Although values in this range may not be stable, it does not make sense to provide min and max values that
        /// are not stable.
        static babelwires::Range<double> getBpmRangeRounded(int decimalPlaces);

        /// The maximum number of decimal places that can be used for rounding BPM values where every value at the
        /// precision is guaranteed to be "stable" (i.e. round trip unchanged).
        static constexpr int c_maxStableDecimalPlaces = 3;

        /// Get the range of representable BPM values all of whose values are "stable" (i.e. round-trip unchanged at the
        /// given precision), which includes the range of BPM values that are commonly used in music. Decimal places
        /// must be between 0 and c_maxStableDecimalPlaces inclusive. This is intended for UI use, where stability is
        /// desirable. The returned ranges are: [4, 7811], [3.6, 2462.6], [3.58, 775.72] and [3.577, 245.316]
        static babelwires::Range<double> getBpmStableRangeRounded(int lowDecimalPlaces);

        auto operator<=>(const TempoValue&) const = default;

        /// Serialization
        std::string serializeToString() const;
        static babelwires::ResultT<TempoValue> deserializeFromString(std::string_view str);

      private:
        /// Construct from a TempoStorage value.
        TempoValue(std::uint32_t microsecondsPerQuaternote);

      private:
        // Default to 120 BPM
        std::uint32_t m_microsecondsPerQuaternote = 500000;
    };
} // namespace bw_music

namespace std {
    template <> struct hash<bw_music::TempoValue> {
        std::size_t operator()(const bw_music::TempoValue& tempo) const noexcept {
            return std::hash<std::uint32_t>()(tempo.getMicrosecondsPerQuaternote());
        }
    };
} // namespace std
