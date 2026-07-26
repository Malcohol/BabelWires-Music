/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/


inline bw_music::PanTrackEvent::PanTrackEvent(ModelDuration timeSinceLastEvent, bw_music::AsymmetricCentredInt value)
    : TrackEvent(timeSinceLastEvent)
    , m_value(value) {
}

inline bw_music::PanTrackEvent::PanTrackEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PanTrackEvent(timeSinceLastEvent, bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(signedNormalizedValue)) {
}

inline bw_music::AsymmetricCentredInt bw_music::PanTrackEvent::getAsymmetricCentredInt() const { return m_value; }

inline double bw_music::PanTrackEvent::getSignedNormalizedValue() const {
    return m_value.getSignedNormalizedValue();
}
