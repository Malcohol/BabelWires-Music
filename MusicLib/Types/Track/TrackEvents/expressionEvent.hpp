/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/Interfaces/sameTypeSubsumingEventMixin.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>

namespace bw_music {
    /// Channel-voice event controlling the expression of a channel.
    struct MUSICLIB_API ExpressionEvent : public TrackEvent, public SameTypeSubsumingEventMixin<ExpressionEvent> {
        DOWNCASTABLE(ExpressionEvent, TrackEvent);
        STREAM_EVENT(ExpressionEvent);
        QUERYABLE_INTERFACE_PROVIDER(TrackEvent, SimultaneousEventSubsumptionInterface);

        /// Construct from a normalized level in the range [0.0, 1.0].
        /// Asserts that the value is in range.
        ExpressionEvent(ModelDuration timeSinceLastEvent, double normalizedLevel);

        /// Construct from a ControllerStorage value.
        ExpressionEvent(ModelDuration timeSinceLastEvent, ControllerStorage value);

        /// Get a value in the range [0.0, 1.0].4
        /// This is the preferred way to obtain the value for calculation.
        double getExpressionAsNormalizedValue() const;

        /// Get the contents as a ControllerStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        ControllerStorage getExpressionStorage() const;

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        ControllerStorage m_expression;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/expressionEvent_inl.hpp>