/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/expressionEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/sustainEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/volumeEvent.hpp>

namespace bw_music {

    struct MUSICLIB_API NotePressureEvent : public TrackEvent {
        DOWNCASTABLE(NotePressureEvent, TrackEvent);
        STREAM_EVENT(NotePressureEvent);

        /// Construct from a signed normalized double value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        NotePressureEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double normalizedPressure);

        /// Construct from a ControllerStorage value.
        NotePressureEvent(ModelDuration timeSinceLastEvent, Pitch pitch, ControllerStorage value);

        Pitch getPitch() const { return m_pitch; }

        /// Get a value in the range [0, 1.0].
        /// This is the preferred way to obtain the value for calculation.
        double getPressureAsNormalizedValue() const;

        /// Get the contents as a CentredControllerStorage value.
        /// This is the preferred way to obtain the value during serialization.
        ControllerStorage getPressureStorage() const;

        std::size_t getHash() const override;
        GroupingInfo getGroupingInfo() const override;

      public:
        template <std::integral Integral> NotePressureEvent(ModelDuration, Pitch, Integral) = delete;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Pitch m_pitch;
        ControllerStorage m_value;
    };

} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/notePressureEvent_inl.hpp>