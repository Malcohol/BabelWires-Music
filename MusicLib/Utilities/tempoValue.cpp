/**
 * A representation of tempo that allows safe round-tripping to and from MIDI tempo storage.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/tempoValue.hpp>

bw_music::TempoValue bw_music::TempoValue::fromBpm(double bpm) {
    assert(std::isfinite(bpm));
    assert(bpm > 0.0);

    const double midiTempoValue = 60'000'000.0 / bpm;
    assert((1.0 <= midiTempoValue) && (midiTempoValue <= static_cast<double>(0xFFFFFFu)));

    return TempoValue(static_cast<std::uint32_t>(std::llround(midiTempoValue)));
}

bw_music::TempoValue bw_music::TempoValue::fromMicrosecondsPerQuaternote(std::uint32_t microsecondsPerQuaternote) {
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
