/**
 * A function which creates a track with modified pitches.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/transposeFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/transposable.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

babelwires::ResultT<bw_music::Track> bw_music::transposeTrack(const Track& trackIn, int pitchOffset, TransposeOutOfRangePolicy outOfRangePolicy) {
    assert(pitchOffset >= -127 && "pitchOffset too low");
    assert(pitchOffset <= 127 && "pitchOffset too high");

    TrackBuilder trackOut;
    ModelDuration durationOfDroppedEvents = 0;
    
    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        TrackEventHolder holder(*it);
        if (auto* transposable = holder->tryInterface<Transposable>()) {
            if (!transposable->transpose(pitchOffset, outOfRangePolicy)) {
                durationOfDroppedEvents += holder->getTimeSinceLastEvent();
                continue;
            }
        }

        if (durationOfDroppedEvents > 0) {
            holder->setTimeSinceLastEvent(holder->getTimeSinceLastEvent() + durationOfDroppedEvents);
            durationOfDroppedEvents = 0;
        }

        trackOut.addEvent(holder.release());
    }

    return trackOut.finishAndGetTrack(trackIn.getDuration());
}

