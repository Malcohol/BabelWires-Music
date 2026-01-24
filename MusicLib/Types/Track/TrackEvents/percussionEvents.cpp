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
#include <typeinfo>
#include <algorithm>

bw_music::TrackEvent::GroupingInfo::Category bw_music::PercussionEvent::s_percussionEventCategory = "Percussion";

void bw_music::PercussionEvent::createEndEvent(TrackEventHolder& dest, ModelDuration timeSinceLastEvent) const {
    dest = PercussionOffEvent(timeSinceLastEvent, m_instrument, m_velocity);
}

bool bw_music::PercussionOnEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOn = static_cast<const PercussionOnEvent&>(other);
    return (m_timeSinceLastEvent == otherOn.m_timeSinceLastEvent) && (m_instrument == otherOn.m_instrument) &&
           (m_velocity == otherOn.m_velocity);
}

std::size_t bw_music::PercussionOnEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOn"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::PercussionOnEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::StartOfGroup};
}

bool bw_music::PercussionOffEvent::operator==(const TrackEvent& other) const {
    if (typeid(other) != typeid(*this)) {
        return false;
    }
    auto otherOff = static_cast<const PercussionOffEvent&>(other);
    return (m_timeSinceLastEvent == otherOff.m_timeSinceLastEvent) && (m_instrument == otherOff.m_instrument) &&
           (m_velocity == otherOff.m_velocity);
}

std::size_t bw_music::PercussionOffEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PercOff"), m_timeSinceLastEvent, m_instrument, m_velocity);
}

bw_music::TrackEvent::GroupingInfo bw_music::PercussionOffEvent::getGroupingInfo() const {
    return {s_percussionEventCategory, m_instrument.toCode(), GroupingInfo::Grouping::EndOfGroup};
}
