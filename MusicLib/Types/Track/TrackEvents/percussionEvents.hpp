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

        void setInstrument(babelwires::ShortId instrument) { m_instrument = instrument; }
        babelwires::ShortId getInstrument() const { return m_instrument; }

        void setVelocity(double velocity) { m_velocity = VelocityStorage::assertFromNormalizedDouble(velocity); }
        double getVelocity() const { return m_velocity.getNormalizedDouble(); }

        void setVelocityStorage(VelocityStorage velocity) { m_velocity = velocity; }
        VelocityStorage getVelocityStorage() const { return m_velocity; }

      protected:
        PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_instrument(instrument)
            , m_velocity(VelocityStorage::assertFromNormalizedDouble(velocity)) {}

        PercussionEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, VelocityStorage velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_instrument(instrument)
            , m_velocity(velocity) {}

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

        PercussionOnEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity)
            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

        PercussionOnEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                          VelocityStorage velocity = c_defaultVelocity)

            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}
        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a percussion event.
    struct MUSICLIB_API PercussionOffEvent : public PercussionEvent {
        DOWNCASTABLE(PercussionOffEvent, PercussionEvent);
        STREAM_EVENT(PercussionOffEvent);
        static constexpr VelocityStorage c_defaultVelocity = MinMaxValue16::assertFromUnsigned<7>(64u);

        PercussionOffEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument, double velocity)
            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

        PercussionOffEvent(ModelDuration timeSinceLastEvent, babelwires::ShortId instrument,
                           VelocityStorage velocity = c_defaultVelocity)
            : PercussionEvent(timeSinceLastEvent, instrument, velocity) {}

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
