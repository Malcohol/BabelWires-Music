/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/expressionEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::ExpressionEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Expression"), m_timeSinceLastEvent, m_expression);
}

bool bw_music::ExpressionEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherExpression = static_cast<const ExpressionEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_expression == otherExpression.m_expression);
}