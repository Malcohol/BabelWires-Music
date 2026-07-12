/**
 * A timed track event for sequence-wide SMF tempo changes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/smfTempoTrackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t smf::TempoTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("SmfTempo"), m_timeSinceLastEvent, m_bpm);
}

bool smf::TempoTrackEvent::doIsEqualTo(const bw_music::TrackEvent& other) const {
    const auto& otherTempo = static_cast<const TempoTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_bpm == otherTempo.m_bpm);
}