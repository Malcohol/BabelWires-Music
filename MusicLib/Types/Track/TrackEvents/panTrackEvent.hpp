/**
 * Channel-voice event controlling the pan of a channel.
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
    /// Channel-voice event controlling the pan of a channel.
    struct MUSICLIB_API PanTrackEvent : public TrackEvent {
        DOWNCASTABLE(PanTrackEvent, TrackEvent);
        STREAM_EVENT(PanTrackEvent);

        /// Construct from an AsymmetricCentredInt value (e.g. as used by MIDI).
        PanTrackEvent(ModelDuration timeSinceLastEvent, AsymmetricCentredInt value);

        /// Construct from a signed normalized double value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        PanTrackEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue);

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
}

#include <MusicLib/Types/Track/TrackEvents/panTrackEvent_inl.hpp>