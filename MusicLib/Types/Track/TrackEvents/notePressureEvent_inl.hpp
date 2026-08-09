/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::NotePressureEvent::NotePressureEvent(ModelDuration timeSinceLastEvent, Pitch pitch, ControllerStorage value)
     : NoteEvent(timeSinceLastEvent, pitch)
     , m_value(value) {}

inline bw_music::NotePressureEvent::NotePressureEvent(ModelDuration timeSinceLastEvent, Pitch pitch, double normalizedPressure)
     : NoteEvent(timeSinceLastEvent, pitch)
     , m_value(ControllerStorage::assertFromNormalizedDouble(normalizedPressure)) {}

inline double bw_music::NotePressureEvent::getPressureAsNormalizedValue() const {
    return m_value.getNormalizedDouble();
}

inline bw_music::ControllerStorage bw_music::NotePressureEvent::getPressureStorage() const {
    return m_value;
}
