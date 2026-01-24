/**
 * Tracks describe a sequence of events, typically notes and chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

#include <BaseLib/Hash/hash.hpp>

bw_music::Track::Track() = default;

bw_music::Track::Track(ModelDuration duration) {
    setDuration(duration);
}

int bw_music::Track::getNumEvents() const {
    return m_blockStream.getNumEvents();
}

bw_music::ModelDuration bw_music::Track::getDuration() const {
    return m_duration;
}

bw_music::ModelDuration bw_music::Track::getTotalEventDuration() const {
    return m_totalEventDuration;
}

void bw_music::Track::setDuration(ModelDuration d) {
    assert((d >= getTotalEventDuration()) && "Attempt to set a duration shorter than the event duration");
    m_duration = d;
}

std::size_t bw_music::Track::getHash() const {
    // The duration can be changed without invalidating cached info. But hash will change.
    std::size_t hash = m_eventHash;
    babelwires::hash::mixInto(hash, m_duration);
    return hash;
}

bool bw_music::Track::operator==(const Value& other) const {
    const Track* const otherTrack = other.tryAs<Track>();
    if (!otherTrack) {
        return false;
    }
    if (otherTrack->getNumEvents() != getNumEvents()) {
        return false;
    }
    if (otherTrack->m_duration != m_duration) {
        return false;
    }
    if (otherTrack->getHash() != getHash()) {
        return false;
    }
    auto thisIt = begin();
    auto otherIt = otherTrack->begin();
    while (thisIt != end()) {
        assert(otherIt != otherTrack->end());
        if (*thisIt != *otherIt) {
            return false;
        }
        ++thisIt;
        ++otherIt;
    }
    assert(otherIt == otherTrack->end());
    return true;
}

void bw_music::Track::addEvent(const TrackEvent& event) {
    onNewEvent(m_blockStream.addEvent(event));
}

void bw_music::Track::addEvent(TrackEvent&& event) {
    onNewEvent(m_blockStream.addEvent(std::move(event)));
};

void bw_music::Track::onNewEvent(const TrackEvent& event) {
    m_totalEventDuration += event.getTimeSinceLastEvent();
    babelwires::hash::mixInto(m_eventHash, event.getHash());
    TrackEvent::GroupingInfo groupingInfo = event.getGroupingInfo();
    if ((groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::NotInGroup) ||
        (groupingInfo.m_grouping == TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
        ++m_numEventGroupsByCategory[groupingInfo.m_category];
    }
}

bw_music::Track::const_iterator bw_music::Track::end() const {
    return m_blockStream.end_impl<TrackEvent>();
}

bw_music::Track::const_iterator bw_music::Track::begin() const {
    return m_blockStream.begin_impl<TrackEvent>();
}

std::reverse_iterator<bw_music::Track::const_iterator> bw_music::Track::rbegin() const {
    return m_blockStream.rbegin_impl<TrackEvent>();
}

std::reverse_iterator<bw_music::Track::const_iterator> bw_music::Track::rend() const {
    return m_blockStream.rend_impl<TrackEvent>();
}


const std::unordered_map<const char*, int>& bw_music::Track::getNumEventGroupsByCategory() const {
    return m_numEventGroupsByCategory;
}
