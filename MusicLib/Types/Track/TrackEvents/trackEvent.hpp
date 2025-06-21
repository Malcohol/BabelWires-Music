/**
 * A TrackEvent is the base class of the events that occur in tracks.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Common/BlockStream/blockStream.hpp>
#include <Common/Cloning/cloneable.hpp>
#include <Common/Utilities/enumFlags.hpp>
#include <MusicLib/musicTypes.hpp>

#include <Common/BlockStream/streamEventHolder.hpp>

namespace bw_music {

    class Track;
    class TrackEvent;
    using TrackEventHolder = babelwires::StreamEventHolder<TrackEvent>;

    /// TrackEvents are usually stored in a BlockStream, which is why they are StreamEvents.
    /// A track can carry arbitrary events. However, tracks often contain events which are
    /// related, For example: a noteOn event, a series of polyphonic aftertouch events,
    /// and a noteOff event, all sharing the same pitch.
    class TrackEvent : public babelwires::StreamEvent {
      public:
        STREAM_EVENT_ABSTRACT(TrackEvent);
        TrackEvent() = default;
        TrackEvent(ModelDuration timeSinceLastEvent) : m_timeSinceLastEvent(timeSinceLastEvent) {}

        /// The amount of time passed since the last event occurred.
        /// This can be 0 if the events are intended to occur at the same time.
        ModelDuration getTimeSinceLastEvent() const { return m_timeSinceLastEvent; }
        void setTimeSinceLastEvent(ModelDuration timeSinceLastEvent) { m_timeSinceLastEvent = timeSinceLastEvent; }

        /// Subclasses should ensure their hashes are distinct from other events
        /// which carry the same data. XORing with typeid(MyEvent).hash_code() should work.
        virtual std::size_t getHash() const {
            // Since this is common to every event, there's no need to distinguish it from hashes of other events.
            return m_timeSinceLastEvent.getHash();
        }

        virtual bool operator==(const TrackEvent& other) const {
            return (typeid(*this) != typeid(other)) ? false : doIsEqualTo(other);
        }

        bool operator!=(const TrackEvent& other) const;

        /// To correctly terminate truncated groups, a start event can be asked to construct a matching end event
        /// of the same category and value. The default implementation asserts;
        // MAYBEDO Consider providing an iterator so the implementation can traverse the group.
        // MAYBEDO If this returns nullptr for a start event, then it means the group cannot be truncated and the
        // group should be removed.
        virtual void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const = 0;

        /// A value which describes how this event can participate in a group of similar events:
        /// For example, a noteOn event, a sequence of after-touch events, and a noteOff event,
        /// all of the same pitch.
        struct EventGroup {
            /// A pointer to a static string can act as a category.
            using Category = const char*;

            /// A category that can be used for events of no particular category.
            static Category s_genericCategory;

            /// Categories are used to identify events which have the same grouping logic.
            /// E.g. notes or chords.
            Category m_category = s_genericCategory;

            auto operator<=>(const EventGroup&) const = default;

            /// A value which is expected to agree for all events in the same group.
            using GroupValue = std::uint64_t;
            constexpr static GroupValue c_notAValue = -1;
            GroupValue m_groupValue = c_notAValue;
        };

        struct GroupingInfo : EventGroup {
            /// A value which determines the way in which this event belongs to a group.
            enum class Grouping : std::uint8_t {
                /// This is a stand-alone event.
                NotInGroup = 0,
                /// This event represents the start of a group.
                StartOfGroup,
                /// This event represents the end of a group.
                EndOfGroup,
                /// This event is (expected to be) inside a group.
                EnclosedInGroup
            } m_grouping = Grouping::NotInGroup;
        };

        /// The default implementation returns values suitable for generic, ungrouped events.
        virtual GroupingInfo getGroupingInfo() const;

        /// If it makes sense, transpose the pitch or pitches described by this event by the given number of semitones.
        /// The default implementation does nothing.
        virtual void transpose(int pitchOffset);

      protected:
        /// Subclasses should override this. They can assume that other is of their type.
        virtual bool doIsEqualTo(const TrackEvent& other) const {
            return m_timeSinceLastEvent == other.m_timeSinceLastEvent;
        }

      protected:
        /// The amount of time passed since the last event occurred.
        /// This can be 0 if the events are intended to occur at the same time.
        ModelDuration m_timeSinceLastEvent;
    };

} // namespace bw_music
