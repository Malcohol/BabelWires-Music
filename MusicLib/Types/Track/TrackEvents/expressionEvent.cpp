/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/expressionEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

namespace {
    struct ExpressionSameTimeSubsumingSession final : bw_music::SimultaneousEventSession {
        EventAndTraversalAction onEarlierEvent(const bw_music::TrackEvent& earlierEvent) override {
            if (earlierEvent.tryAs<bw_music::ExpressionEvent>()) {
                return EventAndTraversalAction::RemoveAndContinue;
            }
            return EventAndTraversalAction::KeepAndContinue;
        }
    };
} // namespace

std::size_t bw_music::ExpressionEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Expression"), m_timeSinceLastEvent, m_expression);
}

std::unique_ptr<bw_music::SimultaneousEventSession> bw_music::ExpressionEvent::createSimultaneousEventSession() {
    return std::make_unique<ExpressionSameTimeSubsumingSession>();
}

bool bw_music::ExpressionEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherExpression = static_cast<const ExpressionEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_expression == otherExpression.m_expression);
}