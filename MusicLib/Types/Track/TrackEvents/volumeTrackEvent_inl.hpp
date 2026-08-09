/**
 * Channel-voice event controlling the volume of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

inline bw_music::VolumeTrackEvent::VolumeTrackEvent(ModelDuration timeSinceLastEvent, bw_music::ControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::VolumeTrackEvent::VolumeTrackEvent(ModelDuration timeSinceLastEvent, double normalizedLevel)
    : VolumeTrackEvent(timeSinceLastEvent, bw_music::ControllerStorage::assertFromNormalizedDouble(normalizedLevel)) {
}

inline double bw_music::VolumeTrackEvent::getLevelAsNormalizedValue() const { return m_value.getNormalizedDouble(); }

inline bw_music::ControllerStorage bw_music::VolumeTrackEvent::getVolumeStorage() const { return m_value; }