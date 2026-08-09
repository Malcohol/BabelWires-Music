/**
 * Channel-voice event controlling the sustain pedal state of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>

namespace bw_music {
    /// Channel-voice event controlling the sustain pedal state of a channel.
    struct MUSICLIB_API SustainEvent : public TrackEvent {
        DOWNCASTABLE(SustainEvent, TrackEvent);
        STREAM_EVENT(SustainEvent);

        /// Construct from a normalized level in the range [0.0, 1.0].
        /// Asserts that the value is in range.
        SustainEvent(ModelDuration timeSinceLastEvent, double normalizedLevel);

        /// Construct from a ControllerStorage value.
        SustainEvent(ModelDuration timeSinceLastEvent, ControllerStorage value);

        /// Get a value in the range [0.0, 1.0].
        double getLevelAsNormalizedValue() const;

        /// Get the contents as a ControllerStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        ControllerStorage getSustainStorage() const;

        /// Return whether the sustain pedal is logically engaged according to the MIDI threshold.
        bool isSustainOn() const;

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        ControllerStorage m_value;
    };
}

#include <MusicLib/Types/Track/TrackEvents/sustainEvent_inl.hpp>