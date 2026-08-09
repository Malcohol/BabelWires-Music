/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/channelVoiceEvents.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::PolyphonicAftertouchEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PolyAftertouch"), m_timeSinceLastEvent, m_pitch,
                                       m_value);
}

bw_music::TrackEvent::GroupingInfo bw_music::PolyphonicAftertouchEvent::getGroupingInfo() const {
    return {NoteEvent::getNoteEventCategory(), m_pitch, GroupRole::EnclosedInGroup};
}

bool bw_music::PolyphonicAftertouchEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherAftertouch = static_cast<const PolyphonicAftertouchEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_pitch == otherAftertouch.m_pitch) && (m_value == otherAftertouch.m_value);
}