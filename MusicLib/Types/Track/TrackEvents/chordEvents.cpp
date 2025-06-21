/**
 * ChordEvents describe musical chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>


#include <Common/Hash/hash.hpp>

#include <sstream>
#include <typeinfo>

bw_music::TrackEvent::GroupingInfo::Category bw_music::ChordEvent::s_chordEventCategory = "Chords";

void bw_music::ChordEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    dest = ChordOffEvent(timeSinceLastEvent);
}

bool bw_music::ChordOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const ChordOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_chord == otherOn.m_chord);
}

std::size_t bw_music::ChordOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOn"), m_timeSinceLastEvent, m_chord.m_root, m_chord.m_chordType);
}

bw_music::TrackEvent::GroupingInfo bw_music::ChordOnEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::StartOfGroup};
}

void bw_music::ChordOnEvent::transpose(int pitchOffset) {
    assert(pitchOffset <= 127);
    assert(pitchOffset >= -127);
    static_assert(static_cast<unsigned int>(PitchClass::Value::NUM_VALUES) == 12);
    unsigned int rootPitch = (static_cast<unsigned int>(m_chord.m_root) + 132 + pitchOffset) % 12;
    m_chord.m_root = static_cast<PitchClass::Value>(rootPitch);
}

bool bw_music::ChordOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const ChordOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent);
}

std::size_t bw_music::ChordOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOff"), m_timeSinceLastEvent);
}

bw_music::TrackEvent::GroupingInfo bw_music::ChordOffEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::EndOfGroup};
}