/**
 * Channel-voice event controlling the pan of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
inline bw_music::PanTrackEvent bw_music::PanTrackEvent::fromUnsigned32(ModelDuration timeSinceLastEvent, std::uint32_t highResValue) {
    return PanTrackEvent(timeSinceLastEvent, highResValue);
}

/// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
template<std::uint8_t numSourceBits>
bw_music::PanTrackEvent bw_music::PanTrackEvent::fromUnsigned(ModelDuration timeSinceLastEvent, std::uint32_t value) {
    return PanTrackEvent(timeSinceLastEvent, bw_music::minCentreMaxScaleUp<numSourceBits, 32>(value));
}

/// Construct from a value in the range [-1.0,.. 0.0,.. 1.0].
inline bw_music::PanTrackEvent bw_music::PanTrackEvent::fromSignedNormalizedDouble(ModelDuration timeSinceLastEvent, double signedNormalizedValue) {
    return PanTrackEvent(timeSinceLastEvent, bw_music::assertScaleSignedNormalizedDoubleto32(signedNormalizedValue));
}

inline std::uint32_t bw_music::PanTrackEvent::getUnsigned32() const { return m_value; }

/// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
template<std::uint8_t numSourceBits>
std::uint32_t bw_music::PanTrackEvent::getUnsigned() const {
    return bw_music::minCentreMaxScaleDown<32, numSourceBits>(m_value);
}

/// Get a value in the range [-1.0,.. 0.0,.. 1.0].
inline double bw_music::PanTrackEvent::getSignedNormalizedValue() const {
    return bw_music::scale32toSignedNormalizedDouble(m_value);
}
