/**
 * A mixin for events that subsume earlier same-time events of the same type.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/TrackEvents/Interfaces/simultaneousEventSubsumptionInterface.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

#include <memory>

namespace bw_music {

    /// This mix-in provides functionality for events that automatically subsume earlier events of the same type
    /// occurring at the same time.
    template <typename DERIVED_EVENT>
    struct SameTypeSubsumingEventMixin : public SimultaneousEventSubsumptionInterface {
        std::unique_ptr<SimultaneousEventSession> createSimultaneousEventSession() override {
            struct SimultaneousEventSubsumingSession final : SimultaneousEventSession {
                EventAndTraversalAction onEarlierEvent(const TrackEvent& earlierEvent) override {
                    if (earlierEvent.tryAs<DERIVED_EVENT>()) {
                        return EventAndTraversalAction::RemoveAndContinue;
                    }
                    return EventAndTraversalAction::KeepAndContinue;
                }
            };

            return std::make_unique<SimultaneousEventSubsumingSession>();
        }
    };

} // namespace bw_music