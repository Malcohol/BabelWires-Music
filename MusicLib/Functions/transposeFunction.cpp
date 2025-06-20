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

bw_music::Track bw_music::transposeTrack(const Track& trackIn, int pitchOffset) {
    assert(pitchOffset >= -127 && "pitchOffset too low");
    assert(pitchOffset <= 127 && "pitchOffset too high");

    TrackBuilder trackOut;
    
    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        TrackEventHolder holder(*it);
        // TODO remove events that go out of range.
        holder->transpose(pitchOffset);
        trackOut.addEvent(holder.release());
    }

    trackOut.setDuration(trackIn.getDuration());
    return trackOut.finishAndGetTrack();
}

