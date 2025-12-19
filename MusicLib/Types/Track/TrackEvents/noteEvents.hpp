/**
 * NoteEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace bw_music {

    /// Base type for note events.
    struct NoteEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(NoteEvent);
        NoteEvent() = default;
        NoteEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch)
            , m_velocity(velocity) {}

        virtual bool transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) override;

        static GroupingInfo::Category s_noteEventCategory;

        void setPitch(Pitch pitch) { m_pitch = pitch; }
        Pitch getPitch() const { return m_pitch; }

        void setVelocity(Velocity velocity) { m_velocity = velocity; }
        Velocity getVelocity() const { return m_velocity; }

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        
        Pitch m_pitch;
        Velocity m_velocity;
    };

    /// The start of a musical note.
    struct NoteOnEvent : public NoteEvent {
        STREAM_EVENT(NoteOnEvent);
        NoteOnEvent() = default;
        NoteOnEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity = 127)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

    /// The end of a musical note.
    struct NoteOffEvent : public NoteEvent {
        STREAM_EVENT(NoteOffEvent);
        NoteOffEvent() = default;
        NoteOffEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity velocity = 64)
            : NoteEvent(timeSinceLastEvent, pitch, velocity) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
