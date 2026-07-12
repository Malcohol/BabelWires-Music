/**
 * A timed track event for sequence-wide SMF tempo changes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

namespace smf {

    struct TempoTrackEvent : public bw_music::TrackEvent {
        DOWNCASTABLE(TempoTrackEvent, bw_music::TrackEvent);
        STREAM_EVENT(TempoTrackEvent);

        TempoTrackEvent(bw_music::ModelDuration timeSinceLastEvent, int bpm)
            : TrackEvent(timeSinceLastEvent)
            , m_bpm(bpm) {}

        int getBpm() const { return m_bpm; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const bw_music::TrackEvent& other) const override;

      private:
        int m_bpm;
    };
} // namespace smf