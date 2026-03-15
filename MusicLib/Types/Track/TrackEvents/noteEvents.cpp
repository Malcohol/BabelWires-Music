/**
 * NoteEvents describe musical notes.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>

#include <BaseLib/Hash/hash.hpp>

#include <algorithm>
#include <sstream>

bw_music::TrackEvent::GroupingInfo::Category bw_music::NoteEvent::s_noteEventCategory = "Notes";

void bw_music::NoteEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    // Create a NoteOffEvent with the same pitch.
    // Use the default NoteOffEvent velocity unless the NoteOnEvent's velocity is lower.
    dest = NoteOffEvent(timeSinceLastEvent, m_pitch, std::min(m_velocity, static_cast<Velocity>(NoteOffEvent::c_defaultVelocity)));
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

std::size_t bw_music::NoteOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOn"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::NoteOnEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::StartOfGroup};
}

std::size_t bw_music::NoteOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NoteOff"), m_timeSinceLastEvent, m_pitch, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::NoteOffEvent::getGroupingInfo() const {
    return {s_noteEventCategory, m_pitch, GroupingInfo::Grouping::EndOfGroup};
}

bool bw_music::NoteEvent::doIsEqualTo(const TrackEvent& other) const {
    auto& otherNote = static_cast<const NoteEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_pitch == otherNote.m_pitch) &&
           (m_velocity == otherNote.m_velocity);
}
