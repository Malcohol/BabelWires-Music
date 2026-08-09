/**
 * PercussionEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/startEventInterface.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace bw_music {

    /// Base of type for percussion events.
    struct MUSICLIB_API PercussionEvent : public TrackEvent {
        DOWNCASTABLE(PercussionEvent, TrackEvent);
        STREAM_EVENT_ABSTRACT(PercussionEvent);

        static GroupKey::Category getPercussionEventCategory();

        void setInstrument(babelwires::ShortId instrument);
        babelwires::ShortId getInstrument() const;

        void setVelocity(double velocity);
        double getVelocity() const;

        void setVelocityStorage(VelocityStorage velocity);
        VelocityStorage getVelocityStorage() const;

      protected:
        PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity);

        PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, VelocityStorage velocity);

        bool doIsEqualTo(const TrackEvent& other) const override;

      protected:
        babelwires::ShortId m_instrument;
        VelocityStorage m_velocity;
    };

    /// The start of a percussion event.
    struct MUSICLIB_API PercussionOnEvent : public PercussionEvent, public StartEventInterface {
        DOWNCASTABLE(PercussionOnEvent, PercussionEvent);
        STREAM_EVENT(PercussionOnEvent);
        QUERYABLE_INTERFACE_PROVIDER(PercussionEvent, StartEventInterface);
        static constexpr VelocityStorage c_defaultVelocity = 0xffff_mmv16;

        PercussionOnEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity);

        PercussionOnEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                          VelocityStorage velocity = c_defaultVelocity);
        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a percussion event.
    struct MUSICLIB_API PercussionOffEvent : public PercussionEvent {
        DOWNCASTABLE(PercussionOffEvent, PercussionEvent);
        STREAM_EVENT(PercussionOffEvent);
        static constexpr VelocityStorage c_defaultVelocity = MinMaxValue16::assertFromUnsigned<7>(64u);

        PercussionOffEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity);

        PercussionOffEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                           VelocityStorage velocity = c_defaultVelocity);

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/percussionEvents_inl.hpp>
