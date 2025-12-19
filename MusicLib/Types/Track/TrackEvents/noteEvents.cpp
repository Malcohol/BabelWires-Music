/**
 * NoteEvents describe musical notes.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>


#include <Common/Hash/hash.hpp>

#include <sstream>
#include <typeinfo>
#include <algorithm>

bw_music::TrackEvent::GroupingInfo::Category bw_music::NoteEvent::s_noteEventCategory = "Notes";

void bw_music::NoteEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    dest = NoteOffEvent(timeSinceLastEvent, m_pitch, m_velocity);
}

bool bw_music::NoteEvent::transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    const auto newPitchOpt = bw_music::transposePitch(static_cast<int>(m_pitch), pitchOffset, outOfRangePolicy);
    if (newPitchOpt.has_value()) {
        m_pitch = static_cast<Pitch>(newPitchOpt.value());
        return true;
    } else {
        return false;
    }
}

bool bw_music::NoteOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const NoteOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_pitch == otherOn.m_pitch) &&
           (m_velocity == otherOn.m_velocity);
}

std::size_t bw_music::NoteOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOn"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::NoteOnEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::StartOfGroup};
}

bool bw_music::NoteOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOff = static_cast<const NoteOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOff.m_timeSinceLastEvent) && (m_pitch == otherOff.m_pitch) &&
           (m_velocity == otherOff.m_velocity);
}

std::size_t bw_music::NoteOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOff"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::NoteOffEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::EndOfGroup};
}
