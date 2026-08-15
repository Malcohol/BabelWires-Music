/**
 * A representation of tempo that allows safe round-tripping to and from MIDI tempo storage.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/tempoValue.hpp>

#include <BaseLib/Utilities/rounding.hpp>

bw_music::TempoValue::TempoValue(std::uint32_t microsecondsPerQuaternote)
    : m_microsecondsPerQuaternote(microsecondsPerQuaternote) {
    assert((1 <= microsecondsPerQuaternote) && (microsecondsPerQuaternote <= 0xFFFFFFu));
}

babelwires::ResultT<bw_music::TempoValue> bw_music::TempoValue::fromBpm(double bpm) {
    if (!std::isfinite(bpm)) {
        return babelwires::ResultT<TempoValue>(babelwires::ErrorStorage("Invalid BPM value"));
    }
    if (bpm <= 0.0) {
        return babelwires::ResultT<TempoValue>(babelwires::ErrorStorage("Negative BPM value"));
    }
    const std::uint32_t midiTempoValue = std::llround(60'000'000.0 / bpm);
    if (midiTempoValue < 1) {
        return babelwires::ResultT<TempoValue>(babelwires::ErrorStorage("BPM value too small to represent"));
    }
    if (midiTempoValue > 0xFFFFFFu) {
        return babelwires::ResultT<TempoValue>(babelwires::ErrorStorage("BPM value too large to represent"));
    }
    
    return TempoValue(midiTempoValue);
}

bw_music::TempoValue bw_music::TempoValue::tryFromBpm(double bpm) {
    if (!std::isfinite(bpm) || (bpm <= 0.0)) {
        return TempoValue(500000); // Use default (120 bpm).
    }
    const std::uint32_t midiTempoValue = std::llround(60'000'000.0 / bpm);
    if (midiTempoValue < 1) {
        return TempoValue(1);
    } else if (midiTempoValue > 0xFFFFFFu) {
        return TempoValue(0xFFFFFFu);
    }

    return TempoValue(midiTempoValue);
}

bw_music::TempoValue bw_music::TempoValue::assertFromBpm(double bpm) {
    assert(std::isfinite(bpm));
    assert(bpm > 0.0);

    const std::uint32_t midiTempoValue = std::llround(60'000'000.0 / bpm);
    assert((1 <= midiTempoValue) && (midiTempoValue <= 0xFFFFFFu));

    return TempoValue(midiTempoValue);
}

babelwires::ResultT<bw_music::TempoValue> bw_music::TempoValue::fromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote) {
    if ((microsecondsPerQuaternote < 1) || (microsecondsPerQuaternote > 0xFFFFFFu)) {
        return babelwires::ResultT<TempoValue>(babelwires::ErrorStorage("MIDI tempo storage value out of range"));
    }
    return TempoValue(microsecondsPerQuaternote);
}

bw_music::TempoValue bw_music::TempoValue::tryFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote) {
    if (microsecondsPerQuaternote < 1) {
        return TempoValue(1);
    } else if (microsecondsPerQuaternote > 0xFFFFFFu) {
        return TempoValue(0xFFFFFFu);
    }
    return TempoValue(microsecondsPerQuaternote);
}

bw_music::TempoValue bw_music::TempoValue::assertFromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote) {
    assert((1 <= microsecondsPerQuaternote) && (microsecondsPerQuaternote <= 0xFFFFFFu));
    return TempoValue(microsecondsPerQuaternote);
}

std::uint32_t bw_music::TempoValue::getMicrosecondsPerQuaternote() const { return m_microsecondsPerQuaternote; }

double bw_music::TempoValue::getBpm() const {
    assert(m_microsecondsPerQuaternote > 0u);
    return 60'000'000.0 / static_cast<double>(m_microsecondsPerQuaternote);
}

double bw_music::TempoValue::getBpmRounded(int decimalPlaces) const {
    assert(decimalPlaces >= 0);
    assert(decimalPlaces <= c_maxPrecisionDecimalPlaces);
    const double bpm = getBpm();
    return babelwires::roundTo(bpm, decimalPlaces);
}

babelwires::Range<double> bw_music::TempoValue::getBpmRangeRounded(int decimalPlaces) {
    assert(decimalPlaces >= 0);
    assert(decimalPlaces <= c_maxPrecisionDecimalPlaces);
    constexpr double maxBpm = 60'000'000.0;
    constexpr double minBpm = 60'000'000.0 / 0xFFFFFFu;
    const double factor = std::pow(10.0, decimalPlaces);
    return {std::ceil(minBpm * factor) / factor, std::floor(maxBpm * factor) / factor};
}

babelwires::Range<double> bw_music::TempoValue::getBpmStableRangeRounded(int lowDecimalPlaces) {
    assert(lowDecimalPlaces >= 0);
    assert(lowDecimalPlaces <= c_maxStableDecimalPlaces);
    // Verified by the TempoValueTest.ReasonableBpmValuesRoundTripUpToTwoDecimalPlaces test.
    const double lowerRanges[] = { 4.0, 3.6, 3.58, 3.577 };
    const double upperRanges[] = { 7811.0, 2462.6, 775.72, 245.316 };
    return {lowerRanges[lowDecimalPlaces], upperRanges[lowDecimalPlaces]};
}
