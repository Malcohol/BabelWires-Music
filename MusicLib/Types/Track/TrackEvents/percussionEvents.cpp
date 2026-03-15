/**
 * PercussionEvents describe musical percussions.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>


#include <BaseLib/Hash/hash.hpp>

#include <sstream>
#include <algorithm>

bw_music::TrackEvent::GroupingInfo::Category bw_music::PercussionEvent::s_percussionEventCategory = "Percussion";

void bw_music::PercussionEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    dest = PercussionOffEvent(timeSinceLastEvent, m_instrument, m_velocity);
}

std::size_t bw_music::PercussionOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOn"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::PercussionOnEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::StartOfGroup};
}

std::size_t bw_music::PercussionOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOff"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::PercussionOffEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::EndOfGroup};
}

bool bw_music::PercussionEvent::doIsEqualTo(const TrackEvent& other) const {
    auto& otherPerc = static_cast<const PercussionEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_instrument == otherPerc.m_instrument) &&
           (m_velocity == otherPerc.m_velocity);
}
