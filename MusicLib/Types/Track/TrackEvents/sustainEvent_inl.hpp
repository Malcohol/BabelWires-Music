/**
 * Channel-voice event controlling the sustain pedal state of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::SustainEvent::SustainEvent(ModelDuration timeSinceLastEvent, bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_sustain(value) {
}

inline bw_music::SustainEvent::SustainEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : SustainEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::SustainEvent::getLevelAsNormalizedValue() const { return m_sustain.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::SustainEvent::getSustainStorage() const { return m_sustain; }

inline bool bw_music::SustainEvent::isSustainOn() const { return m_sustain.getUnsigned<7>() >= 64u; }