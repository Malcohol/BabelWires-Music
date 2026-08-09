/**
 * A timed track event for sequence-wide tempo changes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace bw_music {

    struct MUSICLIB_API TempoEvent : public TrackEvent {
        DOWNCASTABLE(TempoEvent, TrackEvent);
        STREAM_EVENT(TempoEvent);

        TempoEvent(ModelDuration timeSinceLastEvent, int bpm)
            : TrackEvent(timeSinceLastEvent)
            , m_bpm(bpm) {}

        int getBpm() const { return m_bpm; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        int m_bpm;
    };
} // namespace bw_music