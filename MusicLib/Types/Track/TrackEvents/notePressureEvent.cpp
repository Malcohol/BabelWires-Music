/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/notePressureEvent.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::NotePressureEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NotePressure"), m_timeSinceLastEvent, m_pitch,
                                       m_pressure);
}

bool bw_music::NotePressureEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherEvent = static_cast<const NotePressureEvent&>(other);
    return NoteEvent::doIsEqualTo(other) && (m_pressure == otherEvent.m_pressure);
}

bw_music::TrackEvent::GroupingInfo bw_music::NotePressureEvent::getGroupingInfo() const {
    return {NoteEvent::getNoteEventCategory(), m_pitch, GroupRole::EnclosedInGroup};
}