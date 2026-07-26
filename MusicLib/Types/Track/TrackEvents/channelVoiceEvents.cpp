/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/channelVoiceEvents.hpp>

#include <BaseLib/Hash/hash.hpp>

#include <cassert>

namespace {
    constexpr std::int16_t c_minPitchBend = -8192;
    constexpr std::int16_t c_maxPitchBend = 8191;
}

std::size_t bw_music::VolumeTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Volume"), m_timeSinceLastEvent, m_value);
}

bool bw_music::VolumeTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherVolume = static_cast<const VolumeTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherVolume.m_value);
}

std::size_t bw_music::ExpressionTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Expression"), m_timeSinceLastEvent, m_value);
}

bool bw_music::ExpressionTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherExpression = static_cast<const ExpressionTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherExpression.m_value);
}

std::size_t bw_music::SustainTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Sustain"), m_timeSinceLastEvent, m_value);
}

bool bw_music::SustainTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherSustain = static_cast<const SustainTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherSustain.m_value);
}

double bw_music::PitchBendTrackEvent::getNormalizedBend() const {
    assert((c_minPitchBend <= m_bend) && (m_bend <= c_maxPitchBend) && "Pitch bend out of MIDI range");
    if (m_bend == 0) {
        return 0.0;
    }
    if (m_bend < 0) {
        return static_cast<double>(m_bend) / -c_minPitchBend;
    }
    return static_cast<double>(m_bend) / c_maxPitchBend;
}

std::size_t bw_music::PitchBendTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PitchBend"), m_timeSinceLastEvent, m_bend);
}

bool bw_music::PitchBendTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherPitchBend = static_cast<const PitchBendTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_bend == otherPitchBend.m_bend);
}

std::size_t bw_music::ChannelPressureEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("ChannelPressure"), m_timeSinceLastEvent, m_value);
}

bool bw_music::ChannelPressureEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherChannelPressure = static_cast<const ChannelPressureEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherChannelPressure.m_value);
}

std::size_t bw_music::PolyphonicAftertouchEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("PolyAftertouch"), m_timeSinceLastEvent, m_pitch,
                                       m_value);
}

bw_music::TrackEvent::GroupingInfo bw_music::PolyphonicAftertouchEvent::getGroupingInfo() const {
    return {NoteEvent::getNoteEventCategory(), m_pitch, GroupRole::EnclosedInGroup};
}

bool bw_music::PolyphonicAftertouchEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherAftertouch = static_cast<const PolyphonicAftertouchEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_pitch == otherAftertouch.m_pitch) && (m_value == otherAftertouch.m_value);
}