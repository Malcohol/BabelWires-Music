/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <concepts>

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>

namespace bw_music {

    struct MUSICLIB_API PressureEvent : public TrackEvent {
        DOWNCASTABLE(PressureEvent, TrackEvent);
        STREAM_EVENT(PressureEvent);

        /// Construct from a signed normalized double value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        PressureEvent(ModelDuration timeSinceLastEvent, double pressure);

        /// Construct from a ControllerStorage value.
        PressureEvent(ModelDuration timeSinceLastEvent, ControllerStorage value);

        /// Get the contents as a CentredControllerStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        ControllerStorage getPressureStorage() const;

        /// Get a value in the range [0, 1.0].
        /// This is the preferred way to obtain the value for calculation.
        double getPressureAsNormalizedValue() const;

        std::size_t getHash() const override;

      public:
        template <std::integral Integral> PressureEvent(ModelDuration timeSinceLastEvent, Integral) = delete;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        ControllerStorage m_pressure;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/pressureEvent_inl.hpp>