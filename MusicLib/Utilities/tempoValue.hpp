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
        /// Construct a TempoValue from a tempo in beats per minute.
        /// Returns an error if the value cannot be represented (ignoring loss of precision within the accepted range).
        static babelwires::ResultT<TempoValue> fromBpm(double bpm);

        /// Construct a TempoValue from a tempo in beats per minute.
        /// Clamps the value to the representable range if it cannot be represented (ignoring loss of precision within the accepted range).
        static TempoValue tryFromBpm(double bpm);

        /// Construct a TempoValue from a tempo in beats per minute.
        /// Asserts that the value can be represented (ignoring loss of precision within the accepted range).
        static TempoValue assertFromBpm(double bpm);

        /// Construct a TempoValue from an integer in the range [1, 16777215] representing the number of microseconds per quarter note.
        /// Returns an error if the value is out of range.
        static babelwires::ResultT<TempoValue> fromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Construct a TempoValue from an integer in the range [1, 16777215] representing the number of microseconds per quarter note.
        /// Clamps the value if it is out of range.
        static TempoValue tryFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Construct a TempoValue from an integer in the range [1, 16777215] representing the number of microseconds per quarter note.
        /// Asserts that the value is in range.
        static TempoValue assertFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Get the tempo in beats per minute.
        double getBpm() const;

        /// Get the tempo in beats per minute, rounded to the specified number of decimal places.
        double getBpmRounded(int decimalPlaces) const;

        /// Get the contents as an integer in the range [1, 16777215] representing the number of microseconds per quarter note.
        std::uint32_t getMicrosecondsPerQuaternote() const;

        auto operator<=>(const TempoValue&) const = default;

      private:
        /// Construct from a TempoStorage value.
        TempoValue(std::uint32_t microsecondsPerQuaternote);

      private:
        // Default to 120 BPM
        std::uint32_t m_microsecondsPerQuaternote = 500000;
    };
}

namespace std {
    template <> struct hash<bw_music::TempoValue> {
        std::size_t operator()(const bw_music::TempoValue& tempo) const noexcept {
            return std::hash<std::uint32_t>()(tempo.getMicrosecondsPerQuaternote());
        }
    };
} // namespace std
