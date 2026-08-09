/**
 * A timed track event for sequence-wide tempo changes.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <cassert>
#include <cmath>

inline bw_music::TempoEvent::TempoEvent(ModelDuration timeSinceLastEvent, double bpm)
    : TempoEvent(timeSinceLastEvent, bw_music::TempoValue::fromBpm(bpm)) {
}

inline bw_music::TempoEvent::TempoEvent(ModelDuration timeSinceLastEvent, bw_music::TempoValue value)
    : TrackEvent(timeSinceLastEvent)
    , m_tempo(value) {
}

inline double bw_music::TempoEvent::getBpm() const {
    return m_tempo.getBpm();
}

inline bw_music::TempoValue bw_music::TempoEvent::getTempoValue() const { return m_tempo; }
