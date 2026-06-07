/**
 * ChordEvents describe musical chords.
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
#include <MusicLib/chord.hpp>

namespace bw_music {

    /// A ChordEvent describes a musical chord.
    struct MUSICLIB_API ChordEvent : public TrackEvent {
        DOWNCASTABLE(ChordEvent, TrackEvent);
        STREAM_EVENT_ABSTRACT(ChordEvent);
        ChordEvent() = default;
        ChordEvent(ModelDuration timeSinceLastEvent)
            : TrackEvent(timeSinceLastEvent) {}

        static GroupKey::Category getChordEventCategory();

    };

    /// Describes the start of a chord.
    struct MUSICLIB_API ChordOnEvent : public ChordEvent, public Transposable, public StartEventInterface {
        DOWNCASTABLE(ChordOnEvent, ChordEvent);
        STREAM_EVENT(ChordOnEvent);
        QUERYABLE_INTERFACE_PROVIDER(ChordEvent, Transposable, StartEventInterface);
        ChordOnEvent() = default;
        ChordOnEvent(ModelDuration timeSinceLastEvent, Chord chord)
            : ChordEvent(timeSinceLastEvent)
            , m_chord(chord) {}

        void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const override;
        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
        virtual bool transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) override;
        Chord m_chord;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;
    };

    /// The end of a chord.
    struct MUSICLIB_API ChordOffEvent : public ChordEvent {
        DOWNCASTABLE(ChordOffEvent, ChordEvent);
        STREAM_EVENT(ChordOffEvent);
        ChordOffEvent() = default;
        ChordOffEvent(ModelDuration timeSinceLastEvent)
            : ChordEvent(timeSinceLastEvent) {}

        virtual std::size_t getHash() const override;
        virtual GroupingInfo getGroupingInfo() const override;
    };

} // namespace bw_music
