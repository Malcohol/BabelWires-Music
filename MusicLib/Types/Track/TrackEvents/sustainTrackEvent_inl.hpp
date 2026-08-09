/**
 * Channel-voice event controlling the sustain pedal state of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::SustainTrackEvent::SustainTrackEvent(ModelDuration timeSinceLastEvent, bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::SustainTrackEvent::SustainTrackEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : SustainTrackEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::SustainTrackEvent::getLevelAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::SustainTrackEvent::getSustainStorage() const { return m_value; }

inline bool bw_music::SustainTrackEvent::isSustainOn() const { return m_value.getUnsigned<7>() >= 64u; }