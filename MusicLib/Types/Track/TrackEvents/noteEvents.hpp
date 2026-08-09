/**
 * NoteEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/Interfaces/startEventInterface.hpp>
#include <MusicLib/Types/Track/TrackEvents/Interfaces/transposable.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>

namespace bw_music {

    /// Base type for note events.
    class MUSICLIB_API NoteEvent : public TrackEvent, public Transposable {
      public:
        DOWNCASTABLE(NoteEvent, TrackEvent);
        STREAM_EVENT_ABSTRACT(NoteEvent);
        QUERYABLE_INTERFACE_PROVIDER(TrackEvent, Transposable);

        virtual bool transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) override;

        static GroupKey::Category getNoteEventCategory();

        Pitch getPitch() const;
        void setPitch(Pitch pitch);

      protected:
        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch);

        bool doIsEqualTo(const TrackEvent& other) const override;

      protected:
        Pitch m_pitch;
    };

    /// Adds a velocity value to a NoteEvent, used by NoteOnEvent and NoteOffEvent.
    class MUSICLIB_API NoteEventWithVelocity : public NoteEvent {
      public:
        DOWNCASTABLE(NoteEventWithVelocity, NoteEvent);
        STREAM_EVENT_ABSTRACT(NoteEventWithVelocity);

        /// Get the contents as a normalized double value in the range [0, 1.0].
        /// This is the preferred way to obtain the value for calculation.
        double getVelocityAsNormalizedValue() const;

        /// Set the contents as a normalized double value in the range [0, 1.0].
        /// This is the preferred way to set the value from a calculation.
        void setVelocityFromNormalizedValue(double velocity);

        /// Get the contents as a VelocityStorage value.
        /// This is the preferred way to obtain the value during serialization.
        VelocityStorage getVelocityStorage() const;

        /// Set the contents as a VelocityStorage value.
        /// This is the preferred way to set the value during deserialization.
        void setVelocityStorage(VelocityStorage velocity);

      protected:
        NoteEventWithVelocity(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteEventWithVelocity(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity);

        bool doIsEqualTo(const TrackEvent& other) const override;

      protected:
        VelocityStorage m_velocity;
    };

    /// The start of a musical note.
    class MUSICLIB_API NoteOnEvent : public NoteEventWithVelocity, public StartEventInterface {
      public:
        DOWNCASTABLE(NoteOnEvent, NoteEventWithVelocity);
        STREAM_EVENT(NoteOnEvent);
        QUERYABLE_INTERFACE_PROVIDER(NoteEventWithVelocity, StartEventInterface);

        static constexpr VelocityStorage c_defaultVelocity = 0xffff_mmv16;

        /// Construct a note event with the given pitch and velocity (in range [0, 1.0]).
        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity);

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;

      public:
        template <std::integral Integral> NoteOnEvent(ModelDuration, Pitch, Integral) = delete;
    };

    /// The end of a musical note.
    class MUSICLIB_API NoteOffEvent : public NoteEventWithVelocity {
      public:
        DOWNCASTABLE(NoteOffEvent, NoteEventWithVelocity);
        STREAM_EVENT(NoteOffEvent);

        static constexpr VelocityStorage c_defaultVelocity = MinMaxValue16::assertFromUnsigned<7>(64u);

        /// Construct a note event with the given pitch and velocity (in range [0, 1.0]).
        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity);

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;

      public:
        template <std::integral Integral> NoteOffEvent(ModelDuration, Pitch, Integral) = delete;
    };

} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/noteEvents_inl.hpp>
