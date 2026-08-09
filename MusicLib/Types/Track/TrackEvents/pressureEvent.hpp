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

        PressureEvent(ModelDuration timeSinceLastEvent, double pressure);

        PressureEvent(ModelDuration timeSinceLastEvent, ControllerStorage value);

        ControllerStorage getPressureStorage() const;

        double getPressureAsNormalizedValue() const;

        std::size_t getHash() const override;

      public:
        template <std::integral Integral> PressureEvent(ModelDuration timeSinceLastEvent, Integral) = delete;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        ControllerStorage m_value;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/pressureEvent_inl.hpp>