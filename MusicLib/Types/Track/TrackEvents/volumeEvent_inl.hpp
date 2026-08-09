/**
 * Channel-voice event controlling the volume of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::VolumeEvent::VolumeEvent(ModelDuration timeSinceLastEvent, bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::VolumeEvent::VolumeEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : VolumeEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::VolumeEvent::getLevelAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::VolumeEvent::getVolumeStorage() const { return m_value; }