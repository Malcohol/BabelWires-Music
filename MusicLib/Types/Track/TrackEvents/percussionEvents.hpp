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
#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>

namespace bw_music {

    /// Base of type for percussion events.
    struct MUSICLIB_API PercussionEvent : public TrackEvent {
        DOWNCASTABLE(PercussionEvent, TrackEvent);
        STREAM_EVENT_ABSTRACT(PercussionEvent);

        static GroupKey::Category getPercussionEventCategory();

        void setInstrument(babelwires::ShortId instrument);
        babelwires::ShortId getInstrument() const;

        /// Get the contents as a normalized double value in the range [0, 1.0].
        /// This is the preferred way to obtain the value for calculation.
        double getVelocity() const;

        /// Set the contents as a normalized double value in the range [0, 1.0].
        /// This is the preferred way to set the value from a calculation.
        void setVelocity(double velocity);

        /// Get the contents as a VelocityStorage value.
        /// This is the preferred way to obtain the value during serialization.
        VelocityStorage getVelocityStorage() const;

        /// Set the contents as a VelocityStorage value.
        /// This is the preferred way to set the value during deserialization.
        void setVelocityStorage(VelocityStorage velocity);

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
