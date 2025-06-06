/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/excerptFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEventHolder.hpp>

#include <set>

bw_music::Track bw_music::getTrackExcerpt(const Track& trackIn, ModelDuration start,
                                                           ModelDuration duration) {
    Track trackOut;

    ModelDuration end = start + duration;
    ModelDuration timeProcessed;
    ModelDuration timeOfCurrentEvent;

    auto it = trackIn.begin();

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    std::set<Group> groupsOpenAtStart;
    std::set<Group> groupsOpenAtEnd;

    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) < start)) {
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            groupsOpenAtStart.insert(std::make_tuple(info.m_category, info.m_groupValue));
        } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            groupsOpenAtStart.erase(std::make_tuple(info.m_category, info.m_groupValue));
        }
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    bool isFirstEvent = true;
    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) <= end)) {
        bool skip = false;
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            if ((timeProcessed + it->getTimeSinceLastEvent()) != end) {
                groupsOpenAtEnd.insert(std::make_tuple(info.m_category, info.m_groupValue));
            }
            else {
                // Skip groups which start exactly at the end.
                skip = true;
            }
        } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            const Group group = std::make_tuple(info.m_category, info.m_groupValue);
            if (groupsOpenAtStart.find(group) != groupsOpenAtStart.end()) {
                groupsOpenAtStart.erase(group);
                skip = true;
            } else {
                groupsOpenAtEnd.erase(group);
            }
        } else if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup) {
            const Group group = std::make_tuple(info.m_category, info.m_groupValue);
            if (groupsOpenAtStart.find(group) != groupsOpenAtStart.end()) {
                skip = true;
            }
        }

        if (!skip) {
            TrackEventHolder newEvent = *it;
            if (isFirstEvent) {
                newEvent->setTimeSinceLastEvent(newEvent->getTimeSinceLastEvent() + timeProcessed - start);
                isFirstEvent = false;
            }
            trackOut.addEvent(newEvent.release());
        }
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    // Search for the open endOfGroup events and copy them to "now".
    isFirstEvent = true;
    while (!groupsOpenAtEnd.empty() && (it != trackIn.end())) {
        const TrackEvent::GroupingInfo info = it->getGroupingInfo();
        if (info.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            const Group group = std::make_tuple(info.m_category, info.m_groupValue);
            if (groupsOpenAtStart.find(group) == groupsOpenAtStart.end()) {
                groupsOpenAtEnd.erase(group);

                TrackEventHolder newEvent = *it;
                if (isFirstEvent) {
                    newEvent->setTimeSinceLastEvent(end - timeProcessed);
                    isFirstEvent = false;
                } else {
                    newEvent->setTimeSinceLastEvent(0);
                }
                trackOut.addEvent(newEvent.release());
            }
        }
        ++it;
    }
    trackOut.setDuration(duration);
    return trackOut;
}
