/**
 * A queryable interface for track events that can subsume earlier events at the same time.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <BaseLib/Utilities/queryableInterfaceProvider.hpp>

#include <memory>

namespace bw_music {

    class TrackEvent;

    /// A short-lived session used to traverse earlier events at the same time.
    struct MUSICLIB_API SimultaneousEventSession {
        virtual ~SimultaneousEventSession() = default;

        /// The action to take after considering an earlier event during same-time coalescing.
        enum class EventAndTraversalAction { KeepAndContinue, RemoveAndContinue, KeepAndStop, RemoveAndStop };

        /// Called for each earlier event, scanned in reverse order.
        virtual EventAndTraversalAction onEarlierEvent(const TrackEvent& earlierEvent) = 0;
    };

    /// Capability interface for events that can make earlier same-time events redundant.
    /// TrackBuilder uses this to coalesce batched same-time events before performing group sanitization.
    struct MUSICLIB_API SimultaneousEventSubsumptionInterface {
        QUERYABLE_INTERFACE(SimultaneousEventSubsumptionInterface);
        virtual ~SimultaneousEventSubsumptionInterface() = default;

        /// Create a short-lived session for scanning earlier events at the same time.
        /// Note that this method is not const, so this event can be updated based on events seen by the session.
        virtual std::unique_ptr<SimultaneousEventSession> createSimultaneousEventSession() = 0;
    };

} // namespace bw_music