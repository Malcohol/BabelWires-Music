/**
 * A TrackEvent is the base class of the events that occur in tracks.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/BlockStream/blockStream.hpp>
#include <BaseLib/Cloning/cloneable.hpp>
#include <BaseLib/Utilities/enumFlags.hpp>
#include <BaseLib/Utilities/interfaceQueryable.hpp>

#include <MusicLib/Utilities/musicUtilities.hpp>
#include <MusicLib/musicTypes.hpp>

#include <BaseLib/BlockStream/streamEventHolder.hpp>

namespace bw_music {

    class Track;
    class TrackEvent;
    using TrackEventHolder = babelwires::StreamEventHolder<TrackEvent>;

    /// TrackEvents are usually stored in a BlockStream, which is why they are StreamEvents.
    /// A track can carry arbitrary events. However, tracks often contain events which are
    /// related, For example: a noteOn event, a series of polyphonic aftertouch events,
    /// and a noteOff event, all sharing the same pitch.
    class MUSICLIB_API TrackEvent : public babelwires::StreamEvent {
      public:
        DOWNCASTABLE(TrackEvent, babelwires::StreamEvent);
        INTERFACE_QUERYABLE_BASE();
        STREAM_EVENT_ABSTRACT(TrackEvent);
        TrackEvent() = default;
        TrackEvent(ModelDuration timeSinceLastEvent)
            : m_timeSinceLastEvent(timeSinceLastEvent) {}

        /// The amount of time passed since the last event occurred.
        /// This can be 0 if the events are intended to occur at the same time.
        ModelDuration getTimeSinceLastEvent() const { return m_timeSinceLastEvent; }
        void setTimeSinceLastEvent(ModelDuration timeSinceLastEvent) { m_timeSinceLastEvent = timeSinceLastEvent; }

        /// Subclasses should ensure their hashes are distinct from other events
        /// which carry the same data. Mixing in a type-specific string or value should work.
        virtual std::size_t getHash() const;

        bool operator==(const TrackEvent& other) const;
        bool operator!=(const TrackEvent& other) const;

        /// To correctly terminate truncated groups, a start event can be asked to construct a matching end event
        /// of the same category and value. The default implementation asserts;
        // MAYBEDO Consider providing an iterator so the implementation can traverse the group.
        // MAYBEDO If this returns nullptr for a start event, then it means the group cannot be truncated and the
        // group should be removed.
        virtual void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const = 0;

        /// Describes the role an event plays in a group.
        enum class GroupRole : std::uint8_t {
            /// This is a stand-alone event.
            NotInGroup = 0,
            /// This event represents the start of a group.
            StartOfGroup,
            /// This event represents the end of a group.
            EndOfGroup,
            /// This event is (expected to be) inside a group.
            EnclosedInGroup
        };

        /// Identifies a group of related events.
        /// For example, a noteOn event, a sequence of after-touch events, and a noteOff event,
        /// all of the same pitch.
        struct MUSICLIB_API GroupKey {
            /// A pointer to a static string can act as a category.
            using Category = const char*;

            /// A category that can be used for events of no particular category.
            static Category s_genericCategory;

            /// Categories are used to identify events which have the same grouping logic.
            /// E.g. notes or chords.
            Category m_category = s_genericCategory;

            /// A type that is (hopefully) big enough to distinguish all possible event groups of the same category.
            using GroupValue = std::uint64_t;

            /// The default value.
            constexpr static GroupValue c_notAValue = -1;

            /// A value which is expected to agree for all events in the same group.
            GroupValue m_groupValue = c_notAValue;

            auto operator<=>(const GroupKey&) const = default;
        };

        /// The GroupKey and GroupRole information for an event.
        struct MUSICLIB_API GroupingInfo {
            GroupKey m_groupKey;
            GroupRole m_groupRole = GroupRole::NotInGroup;

            bool hasGroup() const { return m_groupRole != GroupRole::NotInGroup; }
        };

        /// The default implementation returns values suitable for generic, ungrouped events.
        virtual GroupingInfo getGroupingInfo() const;

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
