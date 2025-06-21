/**
 * PercussionEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace bw_music {

    /// Base of type for percussion events.
    struct PercussionEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(PercussionEvent);
        PercussionEvent() = default;

        static GroupingInfo::Category s_percussionEventCategory;

        void setInstrument(babelwires::ShortId instrument) { m_instrument = instrument; }
        babelwires::ShortId getInstrument() const { return m_instrument; }

        void setVelocity(Velocity velocity) { m_velocity = velocity; }
        Velocity getVelocity() const { return m_velocity; }

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;

      protected:
        PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, Velocity velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_instrument(instrument)
            , m_velocity(velocity) {}

      protected:
        babelwires::ShortId m_instrument;
        Velocity m_velocity;
    };

    /// The start of a percussion event.
    struct PercussionOnEvent : public PercussionEvent {
        STREAM_EVENT(PercussionOnEvent);
        PercussionOnEvent() = default;
        PercussionOnEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, Velocity velocity = 127)
            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}
        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a percussion event.
    struct PercussionOffEvent : public PercussionEvent {
        STREAM_EVENT(PercussionOffEvent);
        PercussionOffEvent() = default;
        PercussionOffEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, Velocity velocity = 64)
            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
