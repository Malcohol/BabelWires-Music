/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

#include <set>

/// Ensures the following:
/// * All group events are enclosed between start and end events.
/// * All groups must have strictly positive duration
/// * No overlapping groups of the same category and value.
namespace bw_music {
    class TrackBuilder {
      public:
        TrackBuilder();

        /// Add more events to the startTrack.
        TrackBuilder(Track startState);

        /// Add a TrackEvent by moving or copying it into the track.
        void addEvent(const TrackEvent& event);
        void addEvent(TrackEvent&& event);

        void setDuration(ModelDuration d);

        Track finishAndGetTrack();

      private:
        bool onNewEvent(const TrackEvent& event);

        void processEventsAtCurrentTime(bool atEndOfTrack);

        void issueEvent(TrackEvent&& event);
        void issueEvent(const TrackEvent& event);

        bool atEndOfTrack() const;
        ModelDuration getTimeToEndOfTrack() const;
        void endActiveGroups();

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

