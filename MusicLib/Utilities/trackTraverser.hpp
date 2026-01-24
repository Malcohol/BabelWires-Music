/**
 * The TrackTraverser aids in the traversal of a track in time chunks which may lie between events.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/BlockStream/streamEventHolder.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <functional>

namespace bw_music {

    /// This class aids the traversal of a track in time chunks which may lie between events.
    /// When multiplexing the events of several tracks together, it is convenient to use
    /// a traverser for each track.
    template <typename TRACK_ITERATOR = Track::const_iterator> class TrackTraverser {
      public:
        /// Construct a traverser for the given track.
        /// Span must have begin() and end() returning the ITERATOR type.
        template <typename SPAN> TrackTraverser(const Track& track, const SPAN& span);

        /// Set duration to be the maximum between the its current value and the track's duration.
        void leastUpperBoundDuration(ModelDuration& duration) const;

        /// Limit duration to not greater than the time to the next event, if there is one.
        void greatestLowerBoundNextEvent(ModelDuration& duration) const;

        /// Advance the traverser and call the visitor at any events that occur at the new time.
        /// The events may be temporary, so the visitor should not try to store a pointer to them.
        /// time must not exceed the value limited by greatestLowerBoundNextEvent().
        void advance(ModelDuration time,
                     const std::function<void(const typename TRACK_ITERATOR::value_type&)>& eventVisitor);

        /// A vector of events.
        using EventsAtTime = std::vector<babelwires::StreamEventHolder<typename TRACK_ITERATOR::value_type>>;

        /// For convenience, a version of advance is provided which writes events to an array.
        /// The array has to own the events, as the events provided to the visitor can be temporary.
        /// If clearEventsAtTimeOutFirst is true, the events replace the contents of eventsAtTimeOut. Otherwise they
        /// append. time must not exceed the value limited by greatestLowerBoundNextEvent().
        void advance(ModelDuration time, EventsAtTime& eventsAtTimeOut, bool clearEventsAtTimeOutFirst = true);

        /// True if there are more events to process.
        bool hasMoreEvents() const;

        /// Will there be an event at the given time?
        /// time must not exceed the value limited by greatestLowerBoundNextEvent().
        bool isNextEventAt(ModelDuration time) const;

      private:
        /// Seek in the track for the next event which the filter says is interesting.
        void seekNextInterestingEvent();

      private:
        /// The track being traversed.
        const Track& m_track;

        /// Iterator at the event which will be encountered next.
        TRACK_ITERATOR m_iterator;

        /// Iterator at the end of the track.
        TRACK_ITERATOR m_endIterator;

        /// The time until the next event occurs.
        ModelDuration m_timeToNextEvent;
    };

} // namespace bw_music

#include <MusicLib/Utilities/trackTraverser_inl.hpp>