/**
 * A timed track event for sequence-wide tempo changes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Utilities/tempoValue.hpp>

namespace bw_music {
    /// A timed track event for sequence-wide tempo changes.
    struct MUSICLIB_API TempoEvent : public TrackEvent {
        DOWNCASTABLE(TempoEvent, TrackEvent);
        STREAM_EVENT(TempoEvent);

        /// Construct from a tempo value in beats per minute.
        /// Asserts that the value is positive and representable in MIDI tempo storage.
        TempoEvent(ModelDuration timeSinceLastEvent, double bpm);

        /// Construct from a TempoStorage value.
        TempoEvent(ModelDuration timeSinceLastEvent, TempoValue value);

        /// Get the tempo in beats per minute.
        double getBpm() const;

        /// Get the contents as a TempoValue.
        /// This is the preferred way to obtain the value during serialization.
        TempoValue getTempoValue() const;

        std::size_t getHash() const override;

      public:
        template <std::integral Integral> TempoEvent(ModelDuration, Integral) = delete;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        static TempoValue assertTempoStorageFromBpm(double bpm);

        TempoValue m_tempo;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/tempoEvent_inl.hpp>