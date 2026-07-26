/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PitchBendTrackEvent bw_music::PitchBendTrackEvent::fromValue32(ModelDuration timeSinceLastEvent,
                                                                                 std::uint32_t highResValue) {
    return PitchBendTrackEvent(timeSinceLastEvent, highResValue);
}

template<std::uint8_t numSourceBits>
bw_music::PitchBendTrackEvent bw_music::PitchBendTrackEvent::fromValue(ModelDuration timeSinceLastEvent,
                                                                        std::uint32_t value) {
    return PitchBendTrackEvent(timeSinceLastEvent, bw_music::minCentreMaxScaleUp<numSourceBits, 32>(value));
}

inline bw_music::PitchBendTrackEvent bw_music::PitchBendTrackEvent::fromSignedNormalizedDouble(
    ModelDuration timeSinceLastEvent, double signedNormalizedValue) {
    return PitchBendTrackEvent(timeSinceLastEvent, bw_music::assertScaleSignedNormalizedDoubleto32(signedNormalizedValue));
}

inline std::uint32_t bw_music::PitchBendTrackEvent::getValue32() const { return m_value; }

template<std::uint8_t numSourceBits>
std::uint32_t bw_music::PitchBendTrackEvent::getValue() const {
    return bw_music::minCentreMaxScaleDown<32, numSourceBits>(m_value);
}

inline double bw_music::PitchBendTrackEvent::getSignedNormalizedValue() const {
    return bw_music::scale32toSignedNormalizedDouble(m_value);
}