/**
 * Channel-voice event controlling the sustain pedal state of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::SustainEvent::SustainEvent(ModelDuration timeSinceLastEvent, bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::SustainEvent::SustainEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : SustainEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::SustainEvent::getLevelAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::SustainEvent::getSustainStorage() const { return m_value; }

inline bool bw_music::SustainEvent::isSustainOn() const { return m_value.getUnsigned<7>() >= 64u; }