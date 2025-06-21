/**
 * ChordEvents describe musical chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/chord.hpp>

namespace bw_music {

    /// A ChordEvent describes a musical chord.
    struct ChordEvent : public TrackEvent {
        STREAM_EVENT_ABSTRACT(ChordEvent);
        ChordEvent() = default;
        ChordEvent(ModelDuration timeSinceLastEvent)
            : TrackEvent(timeSinceLastEvent) {}

        static GroupingInfo::Category s_chordEventCategory;

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
    };

    /// Describes the start of a chord.
    struct ChordOnEvent : public ChordEvent {
        STREAM_EVENT(ChordOnEvent);
        ChordOnEvent() = default;
        ChordOnEvent(ModelDuration timeSinceLastEvent, Chord chord)
            : ChordEvent(timeSinceLastEvent)
            , m_chord(chord) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
        virtual void transpose(int pitchOffset) override;
        Chord m_chord;
    };

    /// The end of a chord.
    struct ChordOffEvent : public ChordEvent {
        STREAM_EVENT(ChordOffEvent);
        ChordOffEvent() = default;
        ChordOffEvent(ModelDuration timeSinceLastEvent)
            : ChordEvent(timeSinceLastEvent) {}

        virtual bool operator==(const TrackEvent& other) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
