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
    std::map<bw_music::TrackEvent::EventGroup, bw_music::ModelDuration> activeGroups;
    for (const auto& e : track) {
        if (e.getTimeSinceLastEvent() > 0) {
            for (auto& times : activeGroups) {
                times.second += e.getTimeSinceLastEvent();
            }
        }
        const auto groupInfo = e.getGroupingInfo();
        auto activeGroupIt = activeGroups.find(groupInfo);
        switch (groupInfo.m_grouping) {
            case bw_music::TrackEvent::GroupingInfo::Grouping::StartOfGroup: {
                const bool noActiveGroup = (activeGroupIt == activeGroups.end());
                assert((!assertIfInvalid || noActiveGroup) &&
                       "Encountered a start event when there was already a matching group.");
                if (!noActiveGroup) {
                    return false;
                }
                activeGroups.emplace(groupInfo, 0);
                break;
            }
            case bw_music::TrackEvent::GroupingInfo::Grouping::EnclosedInGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an enclosed event when there was no matching group.");
                if (!alreadyAGroup) {
                    return false;
                }
                break;
            }
            case bw_music::TrackEvent::GroupingInfo::Grouping::EndOfGroup: {
                const bool alreadyAGroup = (activeGroupIt != activeGroups.end());
                const bool positiveDurationGroup = (activeGroupIt->second > 0);
                assert((!assertIfInvalid || alreadyAGroup) &&
                       "Encountered an end event when there was no matching group.");
                assert((!assertIfInvalid || positiveDurationGroup) && "Encountered a zero-duration group");
                if (!alreadyAGroup || !positiveDurationGroup) {
                    return false;
                }
                activeGroups.erase(activeGroupIt);
                break;
            }
            case bw_music::TrackEvent::GroupingInfo::Grouping::NotInGroup:
            default:
                break;
        }
    }
    assert(!assertIfInvalid || activeGroups.empty() && "There are still active groups at the end of the track");
    return activeGroups.empty();
}
