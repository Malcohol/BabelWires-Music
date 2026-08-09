/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/pressureEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::PressureEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChannelPressure"), m_timeSinceLastEvent, m_value);
}

bool bw_music::PressureEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherChannelPressure = static_cast<const PressureEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherChannelPressure.m_value);
}
