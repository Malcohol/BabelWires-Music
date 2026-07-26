/**
 * Some utility functions useful for scaling between values of different resolutions, such as MIDI 1.0 and MIDI 2.0.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/asymmetricCentredInt.hpp>

#include <BaseLib/Result/error.hpp>
#include <BaseLib/Hash/hash.hpp>

std::uint32_t bw_music::assertScaleSignedNormalizedDoubleto32(double normalizedValue) {
    assert((normalizedValue >= -1.0) && (normalizedValue <= 1.0) && "Normalized value must be in [-1.0, 1.0]");
    constexpr double centre = 0x80000000;

    if (normalizedValue < 0.0) {
        return static_cast<std::uint32_t>(centre * (1.0 + normalizedValue));
    } else {
        return static_cast<std::uint32_t>(centre + ((centre - 1) * normalizedValue));
    }
}

babelwires::ResultT<std::uint32_t> bw_music::scaleSignedNormalizedDoubleto32(double normalizedValue) {
    if ((normalizedValue < -1.0) || (normalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized signed value must be in [-1.0, 1.0]";
    }
    return assertScaleSignedNormalizedDoubleto32(normalizedValue);
}

std::uint32_t bw_music::tryScaleSignedNormalizedDoubleto32(double normalizedValue) {
    if (normalizedValue < -1.0) {
        return 0; // Clamp to minimum value on error
    } else if (normalizedValue > 1.0) {
        return 0xFFFFFFFF; // Clamp to maximum value on error
    } else {
        return assertScaleSignedNormalizedDoubleto32(normalizedValue);
    }
}

double bw_music::scale32toSignedNormalizedDouble(std::uint32_t highResValue) {
    constexpr double centre = 0x80000000;

    if (highResValue < centre) {
        return (static_cast<double>(highResValue) / centre) - 1.0;
    } else {
        return (static_cast<double>(highResValue - centre) / (centre - 1));
    }
}

std::size_t bw_music::AsymmetricCentredInt::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ACI"), m_value);
}
