/**
 * Channel-voice event controlling the expression of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/expressionTrackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::ExpressionTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Expression"), m_timeSinceLastEvent, m_value);
}

bool bw_music::ExpressionTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherExpression = static_cast<const ExpressionTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherExpression.m_value);
}