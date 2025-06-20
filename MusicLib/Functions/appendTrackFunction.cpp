/**
 * Function which join tracks together sequencially.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/appendTrackFunction.hpp>
#include <MusicLib/Utilities/trackBuilder.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

void bw_music::appendTrack(Track& targetTrack, const Track& sourceTrack) {
    const ModelDuration initialDuration = targetTrack.getDuration();
    const ModelDuration gapAtEnd = targetTrack.getDuration() - targetTrack.getTotalEventDuration();

    TrackBuilder resultTrack(std::move(targetTrack));

    auto it = sourceTrack.begin();
    if (it != sourceTrack.end()) {
        TrackEventHolder firstEventInSequence = *it;
        firstEventInSequence->setTimeSinceLastEvent(firstEventInSequence->getTimeSinceLastEvent() + gapAtEnd);
        resultTrack.addEvent(firstEventInSequence.release());

        for (++it; it != sourceTrack.end(); ++it) {
            resultTrack.addEvent(*it);
        }
    }
    
    resultTrack.setDuration(initialDuration + sourceTrack.getDuration());
    targetTrack = resultTrack.finishAndGetTrack();
}
