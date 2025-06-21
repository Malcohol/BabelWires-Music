/**
 * The TrackValidator checks that the events and groups in a track are correct.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

namespace bw_music {
    class Track;

    namespace Detail {
        bool isTrackValidInternal(const Track& track, bool assertIfInvalid);
    }

    /// Check that the following requirements are met:
    /// * There are no group events outside a group.
    /// * All groups must have strictly positive duration
    inline bool isTrackValid(const Track& track) {
        return Detail::isTrackValidInternal(track, false);
    }

    inline void assertTrackIsValid(const Track& track) {
#ifndef NDEBUG
        Detail::isTrackValidInternal(track, true);
#endif
    }
} // namespace bw_music
