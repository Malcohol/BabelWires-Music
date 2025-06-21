/**
 * Function which merges tracks together.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/mergeFunction.hpp>

#include <MusicLib/Utilities/trackTraverser.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

bw_music::Track bw_music::mergeTracks(const std::vector<const Track*>& sourceTracks) {
    TrackBuilder trackOut;

    bw_music::ModelDuration trackDuration = 0;
    std::vector<TrackTraverser<Track::const_iterator>> traversers;

    const int numTracks = sourceTracks.size();
    traversers.reserve(numTracks);

    for (int i = 0; i < numTracks; ++i) {
        const bw_music::Track& track = *sourceTracks[i];
        traversers.emplace_back(track, track);
        traversers.back().leastUpperBoundDuration(trackDuration);
    }

    bw_music::ModelDuration timeSinceStart = 0;
    while (timeSinceStart < trackDuration) {
        bw_music::ModelDuration timeToNextEvent = trackDuration - timeSinceStart;
        for (int i = 0; i < numTracks; ++i) {
            traversers[i].greatestLowerBoundNextEvent(timeToNextEvent);
        }

        bool isFirstEvent = true;
        for (int i = 0; i < numTracks; ++i) {
            const bw_music::Track& track = *sourceTracks[i];
            traversers[i].advance(timeToNextEvent,
                                  [&isFirstEvent, &timeToNextEvent, &trackOut](const TrackEvent& event) {
                                      TrackEventHolder newEvent = event;
                                      newEvent->setTimeSinceLastEvent(isFirstEvent ? timeToNextEvent : 0);
                                      trackOut.addEvent(newEvent.release());
                                      isFirstEvent = false;
                                  });
        }

        timeSinceStart += timeToNextEvent;
    }

    return trackOut.finishAndGetTrack(trackDuration);
}
