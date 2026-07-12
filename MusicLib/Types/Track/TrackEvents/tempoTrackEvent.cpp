/**
 * A timed track event for sequence-wide tempo changes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/tempoTrackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::TempoTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Tempo"), m_timeSinceLastEvent, m_bpm);
}

bool bw_music::TempoTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherTempo = static_cast<const TempoTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_bpm == otherTempo.m_bpm);
}