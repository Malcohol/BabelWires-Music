/**
 * Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/minCentredMaxValue.hpp>

#include <BaseLib/Hash/hash.hpp>

/// Construct from a value in the range [-1.0,.. 0.0,.. 1.0].
inline bw_music::MinCentredMaxValue bw_music::MinCentredMaxValue::assertFromSignedNormalizedDouble(double signedNormalizedValue) {
    assert((signedNormalizedValue >= -1.0) && (signedNormalizedValue <= 1.0) && "Normalized value must be in [-1.0, 1.0]");
    constexpr double centre = 0x80000000;

    if (signedNormalizedValue < 0.0) {
        return MinCentredMaxValue(static_cast<std::uint32_t>(centre * (1.0 + signedNormalizedValue)));
    } else {
        return MinCentredMaxValue(static_cast<std::uint32_t>(centre + ((centre - 1) * signedNormalizedValue)));
    }
}

babelwires::ResultT<bw_music::MinCentredMaxValue> bw_music::MinCentredMaxValue::fromSignedNormalizedDouble(double signedNormalizedValue) {
    if ((signedNormalizedValue < -1.0) || (signedNormalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized signed value must be in [-1.0, 1.0]";
    }
    return assertFromSignedNormalizedDouble(signedNormalizedValue);
}

bw_music::MinCentredMaxValue bw_music::MinCentredMaxValue::tryFromSignedNormalizedDouble(double signedNormalizedValue) {
    if (signedNormalizedValue < -1.0) {
        return fromUnsigned32(0); // Clamp to minimum value on error
    } else if (signedNormalizedValue > 1.0) {
        return fromUnsigned32(0xFFFFFFFF); // Clamp to maximum value on error
    } else {
        return assertFromSignedNormalizedDouble(signedNormalizedValue);
    }
}

double bw_music::MinCentredMaxValue::getSignedNormalizedValue() const {
    constexpr double centre = 0x80000000;

    if (m_value < centre) {
        return (static_cast<double>(m_value) / centre) - 1.0;
    } else {
        return (static_cast<double>(m_value - centre) / (centre - 1));
    }
}

std::size_t bw_music::MinCentredMaxValue::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ACI"), m_value);
}
