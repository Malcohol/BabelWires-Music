/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <cstdint>

namespace bw_music {

    struct MUSICLIB_API VolumeTrackEvent : public TrackEvent {
        DOWNCASTABLE(VolumeTrackEvent, TrackEvent);
        STREAM_EVENT(VolumeTrackEvent);

        VolumeTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedLevel() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API ExpressionTrackEvent : public TrackEvent {
        DOWNCASTABLE(ExpressionTrackEvent, TrackEvent);
        STREAM_EVENT(ExpressionTrackEvent);

        ExpressionTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedLevel() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API SustainTrackEvent : public TrackEvent {
        DOWNCASTABLE(SustainTrackEvent, TrackEvent);
        STREAM_EVENT(SustainTrackEvent);

        SustainTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        bool isSustainOn() const { return m_value >= 64; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API ChannelPressureEvent : public TrackEvent {
        DOWNCASTABLE(ChannelPressureEvent, TrackEvent);
        STREAM_EVENT(ChannelPressureEvent);

        ChannelPressureEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API PolyphonicAftertouchEvent : public TrackEvent {
        DOWNCASTABLE(PolyphonicAftertouchEvent, TrackEvent);
        STREAM_EVENT(PolyphonicAftertouchEvent);

        PolyphonicAftertouchEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity value)
            : TrackEvent(timeSinceLastEvent)
          , m_pitch(pitch)
          , m_value(value) {}

        Pitch getPitch() const { return m_pitch; }

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;
        GroupingInfo getGroupingInfo() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Pitch m_pitch;
        Velocity m_value;
    };

} // namespace bw_music

