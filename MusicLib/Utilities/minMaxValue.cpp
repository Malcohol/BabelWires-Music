/**
 * A representation of an integer value in the range [0, 0xFFFFFFFF].
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/minMaxValue.hpp>

#include <BaseLib/Hash/hash.hpp>

#include <cassert>

inline bw_music::MinMaxValue bw_music::MinMaxValue::assertFromNormalizedDouble(double normalizedValue) {
    assert((normalizedValue >= 0.0) && (normalizedValue <= 1.0) && "Normalized value must be in [0.0, 1.0]");
    return MinMaxValue(static_cast<std::uint32_t>(0xFFFFFFFFu * normalizedValue));
}

babelwires::ResultT<bw_music::MinMaxValue> bw_music::MinMaxValue::fromNormalizedDouble(double normalizedValue) {
    if ((normalizedValue < 0.0) || (normalizedValue > 1.0)) {
        return babelwires::Error() << "Normalized value must be in [0.0, 1.0]";
    }
    return assertFromNormalizedDouble(normalizedValue);
}

bw_music::MinMaxValue bw_music::MinMaxValue::tryFromNormalizedDouble(double normalizedValue) {
    if (normalizedValue < 0.0) {
        return fromUnsigned32(0u);
    } else if (normalizedValue > 1.0) {
        return fromUnsigned32(0xFFFFFFFFu);
    } else {
        return assertFromNormalizedDouble(normalizedValue);
    }
}

double bw_music::MinMaxValue::getNormalizedDouble() const {
    return static_cast<double>(m_value) / 0xFFFFFFFFu;
}

std::size_t bw_music::MinMaxValue::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("NNI"), m_value);
}