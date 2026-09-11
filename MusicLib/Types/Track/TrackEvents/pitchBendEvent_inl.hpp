/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PitchBendEvent::PitchBendEvent(ModelDuration timeSinceLastEvent,
                                                                                 CentredControllerStorage pitchBend)
                                                                                 : TrackEvent(timeSinceLastEvent)
                                                                                 , m_pitchBend(pitchBend) {}

inline bw_music::PitchBendEvent::PitchBendEvent(
    ModelDuration timeSinceLastEvent, double signedNormalizedValue)
    : PitchBendEvent(timeSinceLastEvent, CentredControllerStorage::assertFromSignedNormalizedDouble(signedNormalizedValue)) {}

inline bw_music::CentredControllerStorage bw_music::PitchBendEvent::getPitchBendStorage() const { return m_pitchBend; }

inline double bw_music::PitchBendEvent::getPitchBendAsSignedNormalizedValue() const {
    return m_pitchBend.getSignedNormalizedValue();
}
