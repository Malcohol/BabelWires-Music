/**
 * Channel-voice event controlling the sustain pedal state of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/sustainEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::SustainEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Sustain"), m_timeSinceLastEvent, m_sustain);
}

bool bw_music::SustainEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherSustain = static_cast<const SustainEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_sustain == otherSustain.m_sustain);
}