/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/panTrackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

#include <cassert>

std::size_t bw_music::PanTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Pan"), m_timeSinceLastEvent, m_value);
}

bool bw_music::PanTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherPan = static_cast<const PanTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherPan.m_value);
}
