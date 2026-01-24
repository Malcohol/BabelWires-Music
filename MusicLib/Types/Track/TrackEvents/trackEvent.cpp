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

bool bw_music::TrackEvent::operator!=(const TrackEvent& other) const {
    return !(*this == other);
}

bw_music::TrackEvent::GroupingInfo bw_music::TrackEvent::getGroupingInfo() const {
    return GroupingInfo();
}

bool bw_music::TrackEvent::transpose(int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    return true;
}
