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
        static babelwires::ResultT<TempoValue> fromBpm(double bpm);

        static TempoValue tryFromBpm(double bpm);

        static TempoValue assertFromBpm(double bpm);

        static babelwires::ResultT<TempoValue> fromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        static TempoValue tryFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        static TempoValue assertFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

        /// Get the tempo in beats per minute.
        double getBpm() const;

        /// Get the tempo in beats per minute, rounded to the specified number of decimal places.
        double getBpmRounded(int decimalPlaces) const;

        void setBpm(double bpm);

        /// Get the contents as a TempoStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        std::uint32_t getMicrosecondsPerQuaternote() const;

        void setMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote);

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
