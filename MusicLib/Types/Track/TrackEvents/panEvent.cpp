/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/panEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

#include <cassert>

std::size_t bw_music::PanEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Pan"), m_timeSinceLastEvent, m_pan);
}

bool bw_music::PanEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherPan = static_cast<const PanEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_pan == otherPan.m_pan);
}
