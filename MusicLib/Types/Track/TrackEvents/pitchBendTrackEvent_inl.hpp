/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(ModelDuration timeSinceLastEvent,
                                                                                 AsymmetricCentredInt aci)
                                                                                 : TrackEvent(timeSinceLastEvent)
                                                                                 , m_value(aci) {}

inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(
    ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PitchBendTrackEvent(timeSinceLastEvent, AsymmetricCentredInt::assertFromSignedNormalizedDouble(signedNormalizedValue)) {}

inline bw_music::AsymmetricCentredInt bw_music::PitchBendTrackEvent::getAsymmetricCentredInt() const { return m_value; }

inline double bw_music::PitchBendTrackEvent::getSignedNormalizedValue() const {
    return m_value.getSignedNormalizedValue();
}
