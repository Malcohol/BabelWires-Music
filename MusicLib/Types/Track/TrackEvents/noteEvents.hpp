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

        void setPitch(Pitch pitch) { m_pitch = pitch; }
        Pitch getPitch() const { return m_pitch; }

        void setVelocity(double velocity) { m_velocity = VelocityStorage::assertFromNormalizedDouble(velocity); }
        double getVelocity() const { return m_velocity.getNormalizedDouble(); }

        void setVelocityStorage(VelocityStorage velocity) { m_velocity = velocity; }
        VelocityStorage getVelocityStorage() const { return m_velocity; }

      protected:
        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch)
            , m_velocity(VelocityStorage::assertFromNormalizedDouble(velocity)) {}

        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch)
            , m_velocity(velocity) {}

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

        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

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

        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double velocity)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, VelocityStorage velocity = c_defaultVelocity)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
