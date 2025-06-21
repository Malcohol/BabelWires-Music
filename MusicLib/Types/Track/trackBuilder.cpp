/**
 * The TrackBuilder ensures tracks are conformant as they are built.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/trackBuilder.hpp>

bw_music::TrackBuilder::TrackBuilder() {}

bw_music::TrackBuilder::TrackBuilder(Track startState)
    : m_track(std::move(startState)) {}

bool bw_music::TrackBuilder::onNewEvent(const TrackEvent& event) {
    const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
    if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) {
        return true;
    }
    if (!m_eventsAtCurrentTime.empty()) {
        m_eventsAtCurrentTime.emplace_back(event);
        return false;
    }
    const auto it = m_activeGroups.find(groupInfo);
    if (it == m_activeGroups.end()) {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_eventsAtCurrentTime.emplace_back(event);
            // This is the necessarily the first event that will end up in m_eventsAtCurrentTime,
            // so the only one that could be carrying any time.
            m_eventsAtCurrentTime.back()->setTimeSinceLastEvent(0);
            return false;
        }
    } else {
        if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            m_activeGroups.erase(it);
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup) {
            return true;
        } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            m_eventsAtCurrentTime.emplace_back(event);
            // This is the necessarily the first event that will end up in m_eventsAtCurrentTime,
            // so the only one that could be carrying any time.
            m_eventsAtCurrentTime.back()->setTimeSinceLastEvent(0);
            return false;
        }
    }
    return false;
}

void bw_music::TrackBuilder::addEvent(const TrackEvent& event) {
    assert(!m_isFinished && "The TrackBuilder is already finished");
    if (event.getTimeSinceLastEvent() > 0) {
        processEventsAtCurrentTime(false);
    }
    if (onNewEvent(event)) {
        issueEvent(event);
    } else {
        m_timeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

void bw_music::TrackBuilder::addEvent(TrackEvent&& event) {
    assert(!m_isFinished && "The TrackBuilder is already finished");
    if (event.getTimeSinceLastEvent() > 0) {
        processEventsAtCurrentTime(false);
    }
    if (onNewEvent(event)) {
        issueEvent(std::move(event));
    } else {
        m_timeSinceLastEvent += event.getTimeSinceLastEvent();
    }
}

void bw_music::TrackBuilder::issueEvent(const TrackEvent& event) {
    if (m_timeSinceLastEvent > 0) {
        TrackEventHolder tmp = event;
        tmp->setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_timeSinceLastEvent);
        m_track.addEvent(tmp.release());
        m_timeSinceLastEvent = 0;
    } else {
        m_track.addEvent(event);
    }
}

void bw_music::TrackBuilder::issueEvent(TrackEvent&& event) {
    if (m_timeSinceLastEvent > 0) {
        event.setTimeSinceLastEvent(event.getTimeSinceLastEvent() + m_timeSinceLastEvent);
        m_timeSinceLastEvent = 0;
    }
    m_track.addEvent(std::move(event));
}

void bw_music::TrackBuilder::processEventsAtCurrentTime(bool atEndOfTrack) {
    // Note: In general, the number of events being processed by this algorithm will be very small.
    unsigned int i = 0;
    while (i < m_eventsAtCurrentTime.size()) {
        if (auto& event = m_eventsAtCurrentTime[i]) {
            const TrackEvent::GroupingInfo groupInfo = event->getGroupingInfo();
            const auto activeGroupIt = m_activeGroups.find(groupInfo);
            if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                // See whether there's a matching end at the same time as the start.
                unsigned int j = m_eventsAtCurrentTime.size() - 1;
                while (j >= i + 1) {
                    if (auto& otherEvent = m_eventsAtCurrentTime[j]) {
                        const TrackEvent::GroupingInfo otherGroupInfo = otherEvent->getGroupingInfo();
                        if (groupInfo == otherGroupInfo) {
                            if (otherGroupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                                // Drop any matching events that were between the start and end.
                                // They either belong to a zero length group or, in the reorder case, we don't know
                                // which group they belong to.
                                for (int k = i + 1; k < j; ++k) {
                                    if (auto& eventInRange = m_eventsAtCurrentTime[k]) {
                                        const TrackEvent::GroupingInfo eventInRangeGroupInfo =
                                            eventInRange->getGroupingInfo();
                                        if (eventInRangeGroupInfo == groupInfo) {
                                            eventInRange.reset();
                                        }
                                    }
                                }
                                if (activeGroupIt == m_activeGroups.end()) {
                                    // Zero-length group: Nothing to do here, since event will be skipped below.
                                } else {
                                    // Assume End/Start out-of-order: Reorder those events
                                    issueEvent(otherEvent.release());
                                    issueEvent(event.release());
                                }
                                break;
                            }
                        }
                    }
                    --j;
                }
                if (j == i) {
                    // Unmatched start: This is the normal case, but we still need to check for start in active group.
                    // Also, don't add start events if we're at the end of the track.
                    if ((activeGroupIt == m_activeGroups.end()) && !atEndOfTrack) {
                        m_activeGroups.emplace(groupInfo);
                        issueEvent(event.release());
                    }
                }
            } else if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                if (activeGroupIt != m_activeGroups.end()) {
                    m_activeGroups.erase(activeGroupIt);
                    issueEvent(event.release());
                }
            } else {
                assert(groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::EnclosedInGroup);
                if (activeGroupIt != m_activeGroups.end()) {
                    issueEvent(event.release());
                }
            }
        }
        ++i;
    }
    m_eventsAtCurrentTime.clear();
}

void bw_music::TrackBuilder::endActiveGroups() {
    if (!m_activeGroups.empty()) {
        ModelDuration initialTime = getTimeToEndOfTrack();
        std::vector<TrackEventHolder> endEventsToAdd;
        endEventsToAdd.reserve(m_activeGroups.size());
        // Seach backwards for the matching start events
        auto it = m_track.rbegin();
        while (it != m_track.rend()) {
            const TrackEvent::GroupingInfo groupInfo = it->getGroupingInfo();
            if (groupInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
                const auto activeGroupIt = m_activeGroups.find(groupInfo);
                if (activeGroupIt != m_activeGroups.end()) {
                    it->createEndEvent(endEventsToAdd.emplace_back(), initialTime);
                    initialTime = 0;
                    m_activeGroups.erase(activeGroupIt);
                    if (m_activeGroups.empty()) {
                        break;
                    }
                }
            }
            ++it;
        }
        assert(m_activeGroups.empty());
        // TODO Need to use event duration here.
        for (auto& endEvent : endEventsToAdd) {
            m_track.addEvent(endEvent.release());
        }
    }
}

bw_music::ModelDuration bw_music::TrackBuilder::getTimeToEndOfTrack() const {
    return m_track.getDuration() - m_track.getTotalEventDuration();
}

bw_music::Track bw_music::TrackBuilder::finishAndGetTrack(ModelDuration d) {
    m_track.setDuration(d);
    processEventsAtCurrentTime(getTimeToEndOfTrack() == m_timeSinceLastEvent);
    endActiveGroups();
    m_isFinished = true;
    return std::move(m_track);
}

bw_music::Track bw_music::TrackBuilder::finishAndGetTrack() {
    return finishAndGetTrack(m_track.getTotalEventDuration());    
}
