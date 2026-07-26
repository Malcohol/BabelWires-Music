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
#include <MusicLib/Utilities/asymmetricCentredInt.hpp>

#include <cstdint>

namespace bw_music {

    /// Channel-voice event controlling the pitch bend of a channel.
    struct MUSICLIB_API PitchBendTrackEvent : public TrackEvent {
        DOWNCASTABLE(PitchBendTrackEvent, TrackEvent);
        STREAM_EVENT(PitchBendTrackEvent);

        /// Construct from an AsymmetricCentredInt value (e.g. as used by MIDI).
        PitchBendTrackEvent(ModelDuration timeSinceLastEvent, AsymmetricCentredInt aci);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        PitchBendTrackEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue);

        /// Get the contents as an AsymmetricCentredInt value (e.g. for use by MIDI).
        AsymmetricCentredInt getAsymmetricCentredInt() const;

        /// Get a value in the range [-1.0, 1.0].
        double getSignedNormalizedValue() const;

        std::size_t getHash() const override;

      protected:

        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        AsymmetricCentredInt m_value;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/pitchBendTrackEvent_inl.hpp>