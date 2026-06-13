/**
 * A TrackEvent is the base class of the events that occur in tracks.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>
#include <BaseLib/Identifiers/registeredIdentifier.hpp>

bw_music::TrackEvent::GroupKey::Category bw_music::TrackEvent::GroupKey::getGenericCategory() {
    return BW_MEDIUM_ID("Generic", "Generic", "3908254a-c900-4426-96c4-34273eb8238d");
}

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
    return {{GroupKey::getGenericCategory()}, GroupRole::NotInGroup};
}
