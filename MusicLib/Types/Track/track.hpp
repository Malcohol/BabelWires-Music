/**
 * A Track is a value which describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/musicTypes.hpp>

#include <BabelWiresLib/TypeSystem/value.hpp>

#include <Common/types.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace bw_music {
    class TrackBuilder;
    class UnsafeTrack;

    /// A track carries a stream of TrackEvents.
    /// Construct a track using a TrackBuilder.
    /// From the point of view of the project, Tracks are not editable: they can be manipulated only using Processors
    /// and can be serialized/deserialized only using SourceFileFormats and TargetFileFormats formats.
    /// The events in a track can belong to groups and those groups are subject to rules, see TrackBuilder.
    class Track : public babelwires::Value {
      public:
        CLONEABLE(Track);

        Track();

        /// Create an empty track with a given duration.
        Track(ModelDuration duration);

      public:
        /// Get the total number of events in the track.
        int getNumEvents() const;

        /// The duration of the track. Note: this can exceed the total duration of the events.
        ModelDuration getDuration() const;

        /// Sets the track to have the given duration, unless that is shorter than the total duration of the events,
        /// in which case, the operation is ignored.
        void setDuration(ModelDuration d);

        /// Determine whether the data in this track and the other are exactly the same.
        /// Note: This is unconcerned with how events are laid out in their streams.
        bool operator==(const Value& other) const override;

        /// Return the total duration of the events in the track (which may be smaller than m_duration).
        ModelDuration getTotalEventDuration() const;

        /// Get a hash corresponding to the state of the track's contents
        std::size_t getHash() const override;

        /// Get a summary of the track contents, by category.
        const std::unordered_map<const char*, int>& getNumEventGroupsByCategory() const;

      private:
        friend TrackBuilder;
        friend UnsafeTrack;

        /// Add a TrackEvent by copying it into the track.
        void addEvent(const TrackEvent& event);

        /// Add a TrackEvent by moving it into the track.
        void addEvent(TrackEvent&& event);

      public:
        using const_iterator = babelwires::BlockStream::Iterator<const babelwires::BlockStream, const TrackEvent>;
        const_iterator begin() const;
        const_iterator end() const;

        std::reverse_iterator<const_iterator> rbegin() const;
        std::reverse_iterator<const_iterator> rend() const;

      protected:
        /// Update the cached info.
        void onNewEvent(const TrackEvent& event);

      protected:
        /// The track's events are stored in a BlockStream.
        babelwires::BlockStream m_blockStream;

        /// The length of the track.
        /// May be longer than the events duration, but may not be shorter.
        ModelDuration m_duration;

        /// The total duration of the events in the track.
        ModelDuration m_totalEventDuration = 0;

        /// A hash computed from the events.
        std::size_t m_eventHash = 0;

        /// A summary of information about the track.
        std::unordered_map<const char*, int> m_numEventGroupsByCategory;
    };

    /// This is only intended for testing tracks.
    class UnsafeTrack : public Track {
      public:
        using Track::addEvent;
    };
} // namespace bw_music
