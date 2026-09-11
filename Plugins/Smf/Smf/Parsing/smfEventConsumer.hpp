/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Smf/Parsing/smfTrackEventConsumer.hpp>

#include <BaseLib/Result/result.hpp>

#include <cstdint>
#include <memory>
#include <span>

namespace smf {

    class SmfEventConsumer {
      public:
        virtual ~SmfEventConsumer() = default;

        virtual babelwires::Result onSequenceStart(std::uint16_t numTracks, std::uint16_t format,
                                                   std::uint16_t division) {
            return {};
        }

        /// Returning nullptr means the track should be ignored.
        virtual std::unique_ptr<SmfTrackEventConsumer> onTrack(std::uint16_t trackIndex) { return nullptr; };
    };

} // namespace smf
