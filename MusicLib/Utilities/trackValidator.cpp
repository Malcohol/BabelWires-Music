/**
 * The TrackValidator checks that the events and groups in a track are correct.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/trackValidator.hpp>

#include <map>

bool bw_music::Detail::isTrackValidInternal(const bw_music::Track& track, bool assertIfInvalid = false) {
    std::map<bw_music::TrackEvent::GroupKey, bw_music::ModelDuration> activeGroups;
    for (const auto& e : track) {
        if (e.getTimeSinceLastEvent() > 0) {
            for (auto& times : activeGroups) {
                times.second += e.getTimeSinceLastEvent();
            }
        }
        const auto groupInfo = e.getGroupingInfo();
        auto activeGroupIt = activeGroups.find(groupInfo.m_groupKey);
        switch (groupInfo.m_groupRole) {
            case bw_music::TrackEvent::GroupRole::StartOfGroup: {
                const bool noActiveGroup = (activeGroupIt == activeGroups.end());
                assert((!assertIfInvalid || noActiveGroup) &&
                       "Encountered a start event when there was already a matching group.");
                if (!noActiveGroup) {
                    return false;
                }
                activeGroups.emplace(groupInfo.m_groupKey, 0);
                break;
            }
            case bw_music::TrackEvent::GroupRole::EnclosedInGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an enclosed event when there was no matching group.");
                if (!alreadyAGroup) {
                    return false;
                }
                break;
            }
            case bw_music::TrackEvent::GroupRole::EndOfGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an end event when there was no matching group.");
                if (!alreadyAGroup) {
                    return false;
                } else {
                    const bool positiveDurationGroup = (activeGroupIt->second > 0);
                    assert((!assertIfInvalid || positiveDurationGroup) && "Encountered a zero-duration group");
                    if (!positiveDurationGroup) {
                        return false;
                    }
                }
                activeGroups.erase(activeGroupIt);
                break;
            }
            case bw_music::TrackEvent::GroupRole::NotInGroup:
            default:
                break;
        }
    }
    assert(!assertIfInvalid || activeGroups.empty() && "There are still active groups at the end of the track");
    return activeGroups.empty();
}
