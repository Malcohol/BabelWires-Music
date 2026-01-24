/**
 * A FilteredTrackIterator provides a way of iterate over a track, skipping events that are not of interest.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/BlockStream/streamEventHolder.hpp>
#include <BaseLib/common.hpp>

namespace bw_music {
    /// Iterate over a track, skipping events that are not of interest.
    /// The iterator provides access to events whose timeSinceLastEvents are adjusted
    /// so they match the last event returned by the iterator.
    /// These are usually constructed in begin/end pairs using the iterateOver<EVENT> function.
    template <typename EVENT = TrackEvent> class FilteredTrackIterator {
      public:
        using value_type = EVENT;

        /// The default implementation just selects by type
        virtual bool isEventOfInterest(const TrackEvent& event) { return event.tryAs<EVENT>(); }

        /// The iterator must be initialized after construction by called initBegin or initEnd.
        FilteredTrackIterator(const Track& track);

        /// Initialize the iterator so it's at the beginning of the track.
        // Implementation note:
        // Subclasses may have an overridden isEventOfInterest and the subclass' v-table and invariants will
        // not be fully set up when the FilteredTrackIterator constructor is called.
        void initBegin();

        /// Initialize the iterator so it's at the end of the track.
        void initEnd();

        FilteredTrackIterator& operator++();
        const EVENT& operator*() const;
        const EVENT* operator->() const;
        bool operator==(const FilteredTrackIterator& other) const;
        bool operator!=(const FilteredTrackIterator& other) const;

      protected:
        void advance();

      private:
        /// This is either at an interesting event, or at end.
        Track::const_iterator m_iterator;
        Track::const_iterator m_end;
        /// If an interesting event needs its time adjusted, it is copied in here.
        babelwires::StreamEventHolder<EVENT> m_event;
        /// Needed for asserts.
        bool m_isInitialized = false;
    };

    /// Return a span of iterators which only return events of the specified type.
    template <typename EVENT> babelwires::Span<FilteredTrackIterator<EVENT>> iterateOver(const Track& track);
} // namespace bw_music

#include <MusicLib/Utilities/filteredTrackIterator_inl.hpp>