/**
 * A function which extracts a section of sequence data from a track.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/excerptFunction.hpp>

#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <set>

bw_music::Track bw_music::getTrackExcerpt(const Track& trackIn, ModelDuration start,
                                                           ModelDuration duration) {
    TrackBuilder trackOut;

    ModelDuration end = start + duration;
    ModelDuration timeProcessed;
    ModelDuration timeOfCurrentEvent;

    auto it = trackIn.begin();

    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) < start)) {
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    bool isFirstEvent = true;
    while ((it != trackIn.end()) && ((timeProcessed + it->getTimeSinceLastEvent()) <= end)) {
        if (isFirstEvent) {
            TrackEventHolder newEvent = *it;
            newEvent->setTimeSinceLastEvent(newEvent->getTimeSinceLastEvent() + timeProcessed - start);
            isFirstEvent = false;
            trackOut.addEvent(newEvent.release());
        } else {
            trackOut.addEvent(*it);
        }
        timeProcessed += it->getTimeSinceLastEvent();
        ++it;
    }

    return trackOut.finishAndGetTrack(duration);
}
