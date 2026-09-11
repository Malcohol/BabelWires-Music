/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/


inline bw_music::PanEvent::PanEvent(ModelDuration timeSinceLastEvent, bw_music::CentredControllerStorage value)
    : TrackEvent(timeSinceLastEvent)
    , m_pan(value) {
}

inline bw_music::PanEvent::PanEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PanEvent(timeSinceLastEvent, bw_music::CentredControllerStorage::assertFromSignedNormalizedDouble(signedNormalizedValue)) {
}

inline bw_music::CentredControllerStorage bw_music::PanEvent::getPanStorage() const { return m_pan; }

inline double bw_music::PanEvent::getPanAsSignedNormalizedValue() const {
    return m_pan.getSignedNormalizedValue();
}
