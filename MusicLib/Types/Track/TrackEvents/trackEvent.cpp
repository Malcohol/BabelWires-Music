/**
 * A TrackEvent is the base class of the events that occur in tracks.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

const char* bw_music::TrackEvent::EventGroup::s_genericCategory = "Generic";

std::size_t bw_music::TrackEvent::getHash() const {
    // Since this is common to every event, there's no need to distinguish it from hashes of other events.
    return m_timeSinceLastEvent.getHash();
}

bool bw_music::TrackEvent::operator==(const TrackEvent& other) const {
    return (getHierarchyId() != other.getHierarchyId()) ? false : doIsEqualTo(other);
}

bool bw_music::TrackEvent::operator!=(const TrackEvent& other) const {
    return !(*this == other);
}

bw_music::TrackEvent::GroupingInfo bw_music::TrackEvent::getGroupingInfo() const {
    return GroupingInfo();
}

bool bw_music::TrackEvent::transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    return true;
}
