/**
 * A function which creates a track with modified pitches.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/transposeFunction.hpp>

#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

bw_music::Track bw_music::transposeTrack(const Track& trackIn, int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    assert(pitchOffset >= -127 && "pitchOffset too low");
    assert(pitchOffset <= 127 && "pitchOffset too high");

    TrackBuilder trackOut;
    ModelDuration durationOfDroppedEvents = 0;
    
    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        TrackEventHolder holder(*it);
        if (holder->transpose(pitchOffset, outOfRangePolicy)) {
            if (durationOfDroppedEvents > 0) {
                holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + durationOfDroppedEvents);
                durationOfDroppedEvents = 0;
            }
            trackOut.addEvent(holder.release());
        } else {
            durationOfDroppedEvents += holder->getTimeSinceLastEvent();
        }
    }

    return trackOut.finishAndGetTrack(trackIn.getDuration());
}

