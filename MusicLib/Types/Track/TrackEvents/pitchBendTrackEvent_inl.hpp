/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(ModelDuration timeSinceLastEvent,
                                                                                 MinCentredMaxValue32 aci)
                                                                                 : TrackEvent(timeSinceLastEvent)
                                                                                 , m_value(aci) {}

inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(
    ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PitchBendTrackEvent(timeSinceLastEvent, MinCentredMaxValue32::assertFromSignedNormalizedDouble(signedNormalizedValue)) {}

inline bw_music::MinCentredMaxValue32 bw_music::PitchBendTrackEvent::getMinCentredMaxValue() const { return m_value; }

inline double bw_music::PitchBendTrackEvent::getSignedNormalizedValue() const {
    return m_value.getSignedNormalizedValue();
}
