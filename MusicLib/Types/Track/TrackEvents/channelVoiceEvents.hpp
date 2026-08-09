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

    using VelocityValue = babelwires::Byte;

    struct MUSICLIB_API ChannelPressureEvent : public TrackEvent {
        DOWNCASTABLE(ChannelPressureEvent, TrackEvent);
        STREAM_EVENT(ChannelPressureEvent);

        ChannelPressureEvent(ModelDuration timeSinceLastEvent, VelocityValue value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        VelocityValue getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        VelocityValue m_value;
    };

    struct MUSICLIB_API PolyphonicAftertouchEvent : public TrackEvent {
        DOWNCASTABLE(PolyphonicAftertouchEvent, TrackEvent);
        STREAM_EVENT(PolyphonicAftertouchEvent);

        PolyphonicAftertouchEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityValue value)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch)
            , m_value(value) {}

        Pitch getPitch() const { return m_pitch; }

        VelocityValue getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;
        GroupingInfo getGroupingInfo() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Pitch m_pitch;
        VelocityValue m_value;
    };

} // namespace bw_music
