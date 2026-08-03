/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/


inline bw_music::PanTrackEvent::PanTrackEvent(ModelDuration timeSinceLastEvent, bw_music::MinCentreMaxValue32 value)
    : TrackEvent(timeSinceLastEvent)
    , m_pan(value) {
}

inline bw_music::PanTrackEvent::PanTrackEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PanTrackEvent(timeSinceLastEvent, bw_music::MinCentreMaxValue32::assertFromSignedNormalizedDouble(signedNormalizedValue)) {
}

inline bw_music::MinCentreMaxValue32 bw_music::PanTrackEvent::getPanAsMinCentreMaxValue() const { return m_pan; }

inline double bw_music::PanTrackEvent::getPanAsSignedNormalizedValue() const {
    return m_pan.getSignedNormalizedValue();
}
