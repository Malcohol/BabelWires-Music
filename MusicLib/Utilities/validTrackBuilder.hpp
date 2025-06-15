/**
 * The ValidTrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <set>

/// Ensures the following:
/// * All groups must have strictly positive duration
/// * No overlapping groups of the same category and value.
namespace bw_music {
    class ValidTrackBuilder {
      public:
        ValidTrackBuilder();

        /// Add a TrackEvent by moving or copying it into the track.
        void addEvent(const TrackEvent& event);
        void addEvent(TrackEvent&& event);

        void setDuration(ModelDuration d);

        Track finishAndGetTrack();

      private:
        bool onNewEvent(const TrackEvent& event);

        void processEventsAtCurrentTime();

        void issueEvent(TrackEvent&& event);
        void issueEvent(const TrackEvent& event);

      private:
        Track m_track;

        std::set<TrackEvent::EventGroup> m_activeGroups;

        /// When events are dropped, their time gets added to the next actual event.
        ModelDuration m_timeSinceLastEvent;

        /// Batch up events at the current time
        std::vector<TrackEventHolder> m_eventsAtCurrentTime;

        bool m_isFinished = false;
    };
} // namespace bw_music

