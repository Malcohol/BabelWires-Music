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
#include <MusicLib/Utilities/minCentreMaxValue.hpp>

#include <cstdint>

namespace bw_music {

    /// Channel-voice event controlling the pitch bend of a channel.
    struct MUSICLIB_API PitchBendEvent : public TrackEvent {
        DOWNCASTABLE(PitchBendEvent, TrackEvent);
        STREAM_EVENT(PitchBendEvent);

        /// Construct from a value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        PitchBendEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue);

        /// Construct from CentredControllerStorage value (e.g. as used by MIDI).
        PitchBendEvent(ModelDuration timeSinceLastEvent, CentredControllerStorage pitchBend);

        /// Get a value in the range [-1.0, 1.0].
        double getPitchBendAsSignedNormalizedValue() const;

        /// Get the contents as a CentredControllerStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        CentredControllerStorage getPitchBendStorage() const;

        std::size_t getHash() const override;

      protected:

        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        CentredControllerStorage m_pitchBend;
    };
} // namespace bw_music

#include <MusicLib/Types/Track/TrackEvents/pitchBendEvent_inl.hpp>