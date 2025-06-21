/**
 * Quantize track events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/quantizeFunction.hpp>

#include <MusicLib/Types/Track/trackBuilder.hpp>

namespace {
    bw_music::ModelDuration getIdealTime(bw_music::ModelDuration time, bw_music::ModelDuration beat) {
        auto [div, mod] = time.divmod(beat);
        if (mod >= beat / 2) {
            ++div;
        }
        return beat * div;
    }
} // namespace

bw_music::Track bw_music::quantize(const Track& trackIn, ModelDuration beat) {

    using Group = std::tuple<TrackEvent::GroupingInfo::Category, TrackEvent::GroupingInfo::GroupValue>;

    TrackBuilder track;

    ModelDuration currentTimeSinceLastEvent;

    // The absolute time of the current event in trackIn.
    ModelDuration trackInAbsoluteTime;
    ModelDuration trackOutAbsoluteTime;

    for (auto it = trackIn.begin(); it != trackIn.end(); ++it) {
        const ModelDuration timeSinceLastEvent = it->getTimeSinceLastEvent();

        ModelDuration newTimeSinceLastEvent;
        if (timeSinceLastEvent > 0) {
            trackInAbsoluteTime += timeSinceLastEvent;
            const ModelDuration idealTime = getIdealTime(trackInAbsoluteTime, beat);
            const ModelDuration newTimeSinceLastEvent = idealTime - trackOutAbsoluteTime;
            TrackEventHolder event = *it;
            event->setTimeSinceLastEvent(newTimeSinceLastEvent);
            track.addEvent(event.release());
            trackOutAbsoluteTime += newTimeSinceLastEvent;
        } else {
            track.addEvent(*it);
        }
    }
    const ModelDuration idealDuration = getIdealTime(trackIn.getDuration(), beat);
    return track.finishAndGetTrack(idealDuration);
}
