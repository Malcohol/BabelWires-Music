/**
 * A queryable interface for track events that can create a matching end event.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/BlockStream/streamEventHolder.hpp>
#include <BaseLib/Utilities/queryableInterfaceProvider.hpp>

#include <MusicLib/musicTypes.hpp>

namespace bw_music {

    class TrackEvent;
    using TrackEventHolder = babelwires::StreamEventHolder<TrackEvent>;

    /// Capability interface expected for all events at the start of a group. 
    /// This allows start events to create matching end events when groups are truncated.
    struct MUSICLIB_API StartEventInterface {
        QUERYABLE_INTERFACE(StartEventInterface);
        virtual ~StartEventInterface() = default;

        /// Construct the matching end event for this event.
        /// To correctly terminate truncated groups, a start event can be asked to construct a matching end event
        /// of the same category and value.
        // MAYBEDO Consider providing an iterator so the implementation can traverse the group.
        // MAYBEDO If this returns nullptr for a start event, then it means the group cannot be truncated and the
        // group should be removed.
        virtual void createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const = 0;
    };

} // namespace bw_music