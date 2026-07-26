/**
 * Channel-voice event controlling the pitch bend of a channel.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Utilities/valueResolutionConversion.hpp>

#include <cstdint>

namespace bw_music {

    /// Channel-voice event controlling the pitch bend of a channel.
    struct MUSICLIB_API PitchBendTrackEvent : public TrackEvent {
        DOWNCASTABLE(PitchBendTrackEvent, TrackEvent);
        STREAM_EVENT(PitchBendTrackEvent);

        /// Construct from a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        static PitchBendTrackEvent fromValue32(ModelDuration timeSinceLastEvent, std::uint32_t highResValue);

        /// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template<std::uint8_t numSourceBits>
        static PitchBendTrackEvent fromValue(ModelDuration timeSinceLastEvent, std::uint32_t value);

        /// Construct from a value in the range [-1.0,.. 0.0,.. 1.0].
        static PitchBendTrackEvent fromSignedNormalizedDouble(ModelDuration timeSinceLastEvent,
                                                              double signedNormalizedValue);

        /// Get a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t getValue32() const;

        /// Get a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template<std::uint8_t numSourceBits>
        std::uint32_t getValue() const;

        /// Get a value in the range [-1.0,.. 0.0,.. 1.0].
        double getSignedNormalizedValue() const;

        std::size_t getHash() const override;

      protected:
        /// Construct from a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        PitchBendTrackEvent(ModelDuration timeSinceLastEvent, std::uint32_t highResValue);

        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        std::uint32_t m_value;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/pitchBendTrackEvent_inl.hpp>