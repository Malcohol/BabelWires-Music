/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::PressureEvent::PressureEvent(ModelDuration timeSinceLastEvent, double value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(ControllerStorage::assertFromNormalizedDouble(value)) {}

inline bw_music::PressureEvent::PressureEvent(ModelDuration timeSinceLastEvent, ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {}

inline bw_music::ControllerStorage bw_music::PressureEvent::getPressureStorage() const {
    return m_value;
}

inline double bw_music::PressureEvent::getPressureAsNormalizedValue() const {
    return m_value.getNormalizedDouble();
}
