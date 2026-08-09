/**
 * NoteEvents describe musical notes.
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
#include <MusicLib/Types/Track/TrackEvents/transposable.hpp>

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
        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity);

        bool doIsEqualTo(const TrackEvent& other) const override;

      protected:
        Pitch m_pitch;
        VelocityStorage m_velocity;
    };

    /// The start of a musical note.
    class MUSICLIB_API NoteOnEvent : public NoteEvent, public StartEventInterface {
      public:
        DOWNCASTABLE(NoteOnEvent, NoteEvent);
        STREAM_EVENT(NoteOnEvent);
        QUERYABLE_INTERFACE_PROVIDER(NoteEvent, StartEventInterface);

        static constexpr VelocityStorage c_defaultVelocity = 0xffff_mmv16;

        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity);

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a musical note.
    class MUSICLIB_API NoteOffEvent : public NoteEvent {
      public:
        DOWNCASTABLE(NoteOffEvent, NoteEvent);
        STREAM_EVENT(NoteOffEvent);

        static constexpr VelocityStorage c_defaultVelocity = MinMaxValue16::assertFromUnsigned<7>(64u);

        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity);

        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity);

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/noteEvents_inl.hpp>
