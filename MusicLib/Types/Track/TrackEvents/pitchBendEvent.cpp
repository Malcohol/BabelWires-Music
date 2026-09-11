/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/pitchBendEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::PitchBendEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PitchBend"), m_timeSinceLastEvent, m_pitchBend);
}

bool bw_music::PitchBendEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherPitchBend = static_cast<const PitchBendEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_pitchBend == otherPitchBend.m_pitchBend);
}
