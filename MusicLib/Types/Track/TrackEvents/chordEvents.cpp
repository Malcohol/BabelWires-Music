/**
 * ChordEvents describe musical chords.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>


#include <BaseLib/Hash/hash.hpp>

#include <sstream>

bw_music::TrackEvent::GroupingInfo::Category bw_music::ChordEvent::s_chordEventCategory = "Chords";

void bw_music::ChordEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    dest = ChordOffEvent(timeSinceLastEvent);
}

std::size_t bw_music::ChordOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOn"), m_timeSinceLastEvent, m_chord.m_root, m_chord.m_chordType);
}

bw_music::TrackEvent::GroupingInfo bw_music::ChordOnEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::StartOfGroup};
}

bool bw_music::ChordOnEvent::transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    // Ignore the policy here, as chord roots are always mapped to within an octave.
    const auto newPitchOpt = bw_music::transposePitch(static_cast<int>(m_chord.m_root), pitchOffset, TransposeOutOfRangePolicy::MapToNearestOctave, 0, 11);
    assert(newPitchOpt.has_value());
    m_chord.m_root = static_cast<PitchClass::Value>(newPitchOpt.value());
    return true;
}

std::size_t bw_music::ChordOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChordOff"), m_timeSinceLastEvent);
}

bw_music::TrackEvent::GroupingInfo bw_music::ChordOffEvent::getGroupingInfo() const {
    return {s_chordEventCategory, 0, GroupingInfo::Grouping::EndOfGroup};
}

bool bw_music::ChordOnEvent::doIsEqualTo(const TrackEvent& other) const {
    auto& otherChord = static_cast<const ChordOnEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_chord == otherChord.m_chord);
}