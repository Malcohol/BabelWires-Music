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
#include <MusicLib/Utilities/minCentreMaxValue.hpp>

#include <cstdint>

namespace bw_music {
    /// Channel-voice event controlling the pan of a channel.
    struct MUSICLIB_API PanEvent : public TrackEvent {
        DOWNCASTABLE(PanEvent, TrackEvent);
        STREAM_EVENT(PanEvent);

        /// Construct from a signed normalized double value in the range [-1.0, 1.0].
        /// Asserts that the value is in range.
        PanEvent(ModelDuration timeSinceLastEvent, double signedNormalizedValue);

        /// Construct from a CentredControllerStorage value.
        PanEvent(ModelDuration timeSinceLastEvent, CentredControllerStorage value);

        /// Get a value in the range [-1.0, 1.0].
        double getPanAsSignedNormalizedValue() const;

        /// Get the contents as a CentredControllerStorage value (e.g. for use by MIDI).
        /// This is the preferred way to obtain the value during serialization.
        CentredControllerStorage getPanStorage() const;

        std::size_t getHash() const override;

      protected:

        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        CentredControllerStorage m_pan;
    };
}

#include <MusicLib/Types/Track/TrackEvents/panEvent_inl.hpp>