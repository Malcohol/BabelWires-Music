/**
 * A function which extracts a list of monophonic tracks from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/monophonicSubtracksFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <algorithm>
#include <set>

ENUM_DEFINE_ENUM_VALUE_SOURCE(bw_music::MonophonicSubtracksPolicyEnum, MONOPHONIC_SUBTRACK_POLICY);

bw_music::MonophonicSubtracksPolicyEnum::MonophonicSubtracksPolicyEnum()
    : babelwires::EnumType(getStaticValueSet(), 0) {}

namespace {
    struct TrackBuilders {
        std::vector<bw_music::TrackBuilder> m_noteTracks;
        bw_music::TrackBuilder m_other;
    };

    struct TrackInfo {
        bw_music::ModelDuration m_timeSinceLastEvent;
        bw_music::TrackEvent::GroupingInfo::GroupValue m_activeValue = bw_music::TrackEvent::GroupingInfo::c_notAValue;
    };

    struct NoteEventInfo {
        /// Notes only.
        bw_music::TrackEventHolder m_event;
        /// This is used to ensure sorting is stable when other factors compare equal.
        int m_originalIndex;
    };

    using PitchSet = std::set<bw_music::TrackEvent::GroupingInfo::GroupValue>;

    const bw_music::TrackEvent::GroupingInfo::Category c_noteCategory = bw_music::NoteEvent::s_noteEventCategory;

    void moveEventToOtherTrack(bw_music::ModelDuration& timeSinceLastEventOther, bw_music::TrackEventHolder& event,
                               TrackBuilders& result) {
        event->setTimeSinceLastEvent(timeSinceLastEventOther);
        result.m_other.addEvent(event.release());
        timeSinceLastEventOther = 0;
    }

    void moveNoteEventToTrack(std::vector<TrackInfo>& trackInfos, NoteEventInfo& noteEvent, int trackToUse,
                              TrackBuilders& result) {
        const bw_music::TrackEvent::GroupingInfo groupInfo = noteEvent.m_event->getGroupingInfo();
        auto& t = trackInfos[trackToUse];
        noteEvent.m_event->setTimeSinceLastEvent(t.m_timeSinceLastEvent);
        result.m_noteTracks[trackToUse].addEvent(noteEvent.m_event.release());
        if (groupInfo.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::StartOfGroup) {
            // Too strong?
            assert(t.m_activeValue == bw_music::TrackEvent::GroupingInfo::c_notAValue);
            t.m_activeValue = groupInfo.m_groupValue;
        } else if (groupInfo.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
            t.m_activeValue = bw_music::TrackEvent::GroupingInfo::c_notAValue;
        }
        t.m_timeSinceLastEvent = 0;
    }

    void evictEvent(std::vector<TrackInfo>& trackInfos, int trackToUse, PitchSet& evictedPitches,
                    TrackBuilders& result) {
        auto& t = trackInfos[trackToUse];
        const bw_music::Pitch pitchToEvict = trackInfos[trackToUse].m_activeValue;
        assert((evictedPitches.find(pitchToEvict) == evictedPitches.end()) && "Evicting an already evicted pitch");
        evictedPitches.insert(pitchToEvict);
        result.m_noteTracks[trackToUse].addEvent(bw_music::NoteOffEvent{t.m_timeSinceLastEvent, pitchToEvict});
        t.m_activeValue = bw_music::TrackEvent::GroupingInfo::c_notAValue;
        t.m_timeSinceLastEvent = 0;
    }

    /// Returns [trackToUse, shouldEvict]
    std::tuple<int, bool> getTrackToUse(const std::vector<TrackInfo>& trackInfos,
                                        const bw_music::TrackEventHolder& event,
                                        bw_music::MonophonicSubtracksPolicyEnum::Value policy) {
        const bool isEvicting = (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::HighEv) ||
                                (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::LowEv);
        const bool preferHigherPitches = (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::High) ||
                                         (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::HighEv);
        int trackToUse = -1;
        bool shouldEvict = false;
        const auto& groupInfo = event->getGroupingInfo();
        assert(groupInfo.m_category == c_noteCategory);
        for (int i = 0; i < trackInfos.size(); ++i) {
            auto& t = trackInfos[i];
            if (t.m_activeValue == groupInfo.m_groupValue) {
                trackToUse = i;
                break;
            } else if ((trackToUse == -1) && (t.m_activeValue == bw_music::TrackEvent::GroupingInfo::c_notAValue) &&
                        (groupInfo.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
                trackToUse = i;
            }
        }
        if (isEvicting && (trackToUse == -1)) {
            bw_music::Pitch bestEvictablePitch = preferHigherPitches ? std::numeric_limits<bw_music::Pitch>::max()
                                                                     : std::numeric_limits<bw_music::Pitch>::min();
            for (int i = 0; i < trackInfos.size(); ++i) {
                auto& t = trackInfos[i];
                if (((groupInfo.m_groupValue > t.m_activeValue) == preferHigherPitches) &&
                    ((t.m_activeValue < bestEvictablePitch) == preferHigherPitches)) {
                    trackToUse = i;
                    bestEvictablePitch = t.m_activeValue;
                    shouldEvict = true;
                }
            }
        }
        return {trackToUse, shouldEvict};
    }

    void assignNoteEventsToTracks(std::vector<TrackInfo>& trackInfos, bw_music::ModelDuration& timeSinceLastEventOther,
                                  std::vector<NoteEventInfo>& noteEvents, PitchSet& evictedPitches,
                                  bw_music::MonophonicSubtracksPolicyEnum::Value policy,
                                  TrackBuilders& result) {
        const bool preferHigherPitches = (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::High) ||
                                         (policy == bw_music::MonophonicSubtracksPolicyEnum::Value::HighEv);
        const auto lessThan = [preferHigherPitches](NoteEventInfo& a, NoteEventInfo& b) {
            const auto& groupInfoA = a.m_event->getGroupingInfo();
            const auto& groupInfoB = b.m_event->getGroupingInfo();
            if ((groupInfoA.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::EndOfGroup) &&
                (groupInfoB.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::StartOfGroup)) {
                return true;
            }
            if ((groupInfoA.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::StartOfGroup) &&
                (groupInfoB.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::EndOfGroup)) {
                return false;
            }
            if (groupInfoA.m_groupValue > groupInfoB.m_groupValue) {
                return preferHigherPitches;
            }
            if (groupInfoA.m_groupValue < groupInfoB.m_groupValue) {
                return !preferHigherPitches;
            }
            // Preserve the original order in other cases.
            return a.m_originalIndex < b.m_originalIndex;
        };

        std::sort(noteEvents.begin(), noteEvents.end(), lessThan);

        for (auto& noteEvent : noteEvents) {
            const bw_music::TrackEvent::GroupingInfo groupInfo = noteEvent.m_event->getGroupingInfo();
            const auto it = evictedPitches.find(groupInfo.m_groupValue);
            if (it != evictedPitches.end()) {
                if (groupInfo.m_grouping == bw_music::TrackEvent::GroupingInfo::Grouping::EndOfGroup) {
                    evictedPitches.erase(it);
                } // else ignore this event.
            } else {
                const auto [trackToUse, shouldEvict] = getTrackToUse(trackInfos, noteEvent.m_event, policy);
                if (trackToUse != -1) {
                    if (shouldEvict) {
                        evictEvent(trackInfos, trackToUse, evictedPitches, result);
                    }
                    moveNoteEventToTrack(trackInfos, noteEvent, trackToUse, result);
                } else {
                    moveEventToOtherTrack(timeSinceLastEventOther, noteEvent.m_event, result);
                }
            }
        }
    }
} // namespace

bw_music::MonophonicSubtracksResult bw_music::getMonophonicSubtracks(const Track& trackIn, int numTracks,
                                                                     MonophonicSubtracksPolicyEnum::Value policy) {
    assert(numTracks > 0);
    TrackBuilders builders;

    std::vector<TrackInfo> trackInfos;
    trackInfos.resize(numTracks);
    builders.m_noteTracks.resize(numTracks);
    ModelDuration timeSinceLastEventOther;

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    std::vector<NoteEventInfo> noteEventsNow;
    PitchSet evictedPitches;

    for (auto& event : trackIn) {
        if (event.getTimeSinceLastEvent() != 0) {
            assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, evictedPitches, policy,
                                     builders);
            noteEventsNow.clear();

            for (auto& t : trackInfos) {
                t.m_timeSinceLastEvent += event.getTimeSinceLastEvent();
            }
            timeSinceLastEventOther += event.getTimeSinceLastEvent();
        }

        const TrackEvent::GroupingInfo groupInfo = event.getGroupingInfo();
        if (groupInfo.m_category == c_noteCategory) {
            NoteEventInfo& noteInfo = noteEventsNow.emplace_back();
            noteInfo.m_event = event;
            noteInfo.m_originalIndex = noteEventsNow.size() - 1;
        } else {
            TrackEventHolder otherEvent = event;
            moveEventToOtherTrack(timeSinceLastEventOther, otherEvent, builders);
        }
    }
    assignNoteEventsToTracks(trackInfos, timeSinceLastEventOther, noteEventsNow, evictedPitches, policy, builders);

    bw_music::MonophonicSubtracksResult result;
    result.m_noteTracks.reserve(builders.m_noteTracks.size());
    for (auto& trackBuilder : builders.m_noteTracks) {
        trackBuilder.setDuration(trackIn.getDuration());
        result.m_noteTracks.emplace_back(trackBuilder.finishAndGetTrack());
    }
    builders.m_other.setDuration(trackIn.getDuration());
    result.m_other = builders.m_other.finishAndGetTrack();
    return result;
}
