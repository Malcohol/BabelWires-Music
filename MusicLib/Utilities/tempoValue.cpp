/**
 * A representation of tempo that allows safe round-tripping to and from MIDI tempo storage.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/tempoValue.hpp>

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

void bw_music::TempoValue::setMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote) {
    assert((1 <= microsecondsPerQuaternote) && (microsecondsPerQuaternote <= 0xFFFFFFu));
    m_microsecondsPerQuaternote = microsecondsPerQuaternote;
}

double bw_music::TempoValue::getBpm() const {
    assert(m_microsecondsPerQuaternote > 0u);
    return 60'000'000.0 / static_cast<double>(m_microsecondsPerQuaternote);
}

double bw_music::TempoValue::getBpmRounded(int decimalPlaces) const {
    assert(decimalPlaces >= 0);
    const double bpm = getBpm();
    const double factor = std::pow(10.0, static_cast<double>(decimalPlaces));
    return std::round(bpm * factor) / factor;
}

void bw_music::TempoValue::setBpm(double bpm) {
    assert(std::isfinite(bpm));
    assert(bpm > 0.0);

    const double midiTempoValue = 60'000'000.0 / bpm;
    assert((1.0 <= midiTempoValue) && (midiTempoValue <= static_cast<double>(0xFFFFFFu)));

    m_microsecondsPerQuaternote = static_cast<std::uint32_t>(std::llround(midiTempoValue));
}

bw_music::TempoValue::TempoValue(std::uint32_t microsecondsPerQuaternote)
    : m_microsecondsPerQuaternote(microsecondsPerQuaternote) {
    assert((1 <= microsecondsPerQuaternote) && (microsecondsPerQuaternote <= 0xFFFFFFu));
}
