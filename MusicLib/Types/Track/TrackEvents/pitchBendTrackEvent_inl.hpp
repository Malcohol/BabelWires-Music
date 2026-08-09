/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(ModelDuration timeSinceLastEvent,
                                                                                 CentredControllerStorage aci)
                                                                                 : TrackEvent(timeSinceLastEvent)
                                                                                 , m_pitchBend(aci) {}

inline bw_music::PitchBendTrackEvent::PitchBendTrackEvent(
    ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PitchBendTrackEvent(timeSinceLastEvent, CentredControllerStorage::assertFromSignedNormalizedDouble(signedNormalizedValue)) {}

inline bw_music::CentredControllerStorage bw_music::PitchBendTrackEvent::getPitchBendStorage() const { return m_pitchBend; }

inline double bw_music::PitchBendTrackEvent::getPitchBendAsSignedNormalizedValue() const {
    return m_pitchBend.getSignedNormalizedValue();
}
