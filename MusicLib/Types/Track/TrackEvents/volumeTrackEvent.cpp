/**
 * Channel-voice event controlling the volume of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/TrackEvents/volumeTrackEvent.hpp>

#include <BaseLib/Hash/hash.hpp>

std::size_t bw_music::VolumeTrackEvent::getHash() const {
    return babelwires::hash::mixtureOf(static_cast<const char*>("Volume"), m_timeSinceLastEvent, m_value);
}

bool bw_music::VolumeTrackEvent::doIsEqualTo(const TrackEvent& other) const {
    const auto& otherVolume = static_cast<const VolumeTrackEvent&>(other);
    return TrackEvent::doIsEqualTo(other) && (m_value == otherVolume.m_value);
}