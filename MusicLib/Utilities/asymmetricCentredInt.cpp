/**
 * Some utility functions useful for scaling between values of different resolutions, such as MIDI 1.0 and MIDI 2.0.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/asymmetricCentredInt.hpp>

#include <BaseLib/Hash/hash.hpp>

/// Construct from a value in the range [-1.0,.. 0.0,.. 1.0].
inline bw_music::AsymmetricCentredInt bw_music::AsymmetricCentredInt::assertFromSignedNormalizedDouble(double signedNormalizedValue) {
    assert((signedNormalizedValue >= -1.0) && (signedNormalizedValue <= 1.0) && "Normalized value must be in [-1.0, 1.0]");
    constexpr double centre = 0x80000000;

    if (signedNormalizedValue < 0.0) {
        return AsymmetricCentredInt(static_cast<std::uint32_t>(centre * (1.0 + signedNormalizedValue)));
    } else {
        return AsymmetricCentredInt(static_cast<std::uint32_t>(centre + ((centre - 1) * signedNormalizedValue)));
    }
}

babelwires::ResultT<bw_music::AsymmetricCentredInt> bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(double signedNormalizedValue) {
    if ((signedNormalizedValue < -1.0) || (signedNormalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized signed value must be in [-1.0, 1.0]";
    }
    return assertFromSignedNormalizedDouble(signedNormalizedValue);
}

bw_music::AsymmetricCentredInt bw_music::AsymmetricCentredInt::tryFromSignedNormalizedDouble(double signedNormalizedValue) {
    if (signedNormalizedValue < -1.0) {
        return fromUnsigned32(0); // Clamp to minimum value on error
    } else if (signedNormalizedValue > 1.0) {
        return fromUnsigned32(0xFFFFFFFF); // Clamp to maximum value on error
    } else {
        return assertFromSignedNormalizedDouble(signedNormalizedValue);
    }
}

double bw_music::AsymmetricCentredInt::getSignedNormalizedValue() const {
    constexpr double centre = 0x80000000;

    if (m_value < centre) {
        return (static_cast<double>(m_value) / centre) - 1.0;
    } else {
        return (static_cast<double>(m_value - centre) / (centre - 1));
    }
}

std::size_t bw_music::AsymmetricCentredInt::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ACI"), m_value);
}
