/**
 * Timed channel-voice events with reusable musical semantics.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Utilities/valueResolutionConversion.hpp>

#include <cstdint>

namespace bw_music {

    struct MUSICLIB_API PanTrackEvent : public TrackEvent {
        DOWNCASTABLE(PanTrackEvent, TrackEvent);
        STREAM_EVENT(PanTrackEvent);

        /// Construct from a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        static PanTrackEvent fromValue32(ModelDuration timeSinceLastEvent, std::uint32_t highResValue) {
            return PanTrackEvent(timeSinceLastEvent, highResValue);
        }

        /// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template<std::uint8_t numSourceBits>
        static PanTrackEvent fromValue(ModelDuration timeSinceLastEvent, std::uint32_t value) {
            return PanTrackEvent(timeSinceLastEvent, bw_music::minCentreMaxScaleUp<numSourceBits, 32>(value));
        }

        /// Construct from a value in the range [-1.0,.. 0.0,.. 1.0].
        static PanTrackEvent fromSignedNormalizedDouble(ModelDuration timeSinceLastEvent, double signedNormalizedValue) {
            return PanTrackEvent(timeSinceLastEvent, bw_music::assertScaleSignedNormalizedDoubleto32(signedNormalizedValue));
        }

        /// Get a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        std::uint32_t getValue32() const;

        /// Construct from a value in the range [0,.. 2^(numSourceBits - 1),.. (2^numSourceBits) - 1].
        template<std::uint8_t numSourceBits>
        std::uint32_t getValue() const {
            return bw_music::minCentreMaxScaleDown<32, numSourceBits>(m_value);
        }

        /// Get a value in the range [-1.0,.. 0.0,.. 1.0].
        double getSignedNormalizedValue() const {
            return bw_music::scale32toSignedNormalizedDouble(m_value);
        }

        std::size_t getHash() const override;

      protected:
        /// Construct from a value in the range [0,.. 0x80000000,.. 0xFFFFFFFF].
        PanTrackEvent(ModelDuration timeSinceLastEvent, std::uint32_t highResValue);

        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        std::uint32_t m_value;
    };

    struct MUSICLIB_API VolumeTrackEvent : public TrackEvent {
        DOWNCASTABLE(VolumeTrackEvent, TrackEvent);
        STREAM_EVENT(VolumeTrackEvent);

        VolumeTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedLevel() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API ExpressionTrackEvent : public TrackEvent {
        DOWNCASTABLE(ExpressionTrackEvent, TrackEvent);
        STREAM_EVENT(ExpressionTrackEvent);

        ExpressionTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedLevel() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API SustainTrackEvent : public TrackEvent {
        DOWNCASTABLE(SustainTrackEvent, TrackEvent);
        STREAM_EVENT(SustainTrackEvent);

        SustainTrackEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        bool isSustainOn() const { return m_value >= 64; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API PitchBendTrackEvent : public TrackEvent {
        DOWNCASTABLE(PitchBendTrackEvent, TrackEvent);
        STREAM_EVENT(PitchBendTrackEvent);

        PitchBendTrackEvent(ModelDuration timeSinceLastEvent, std::int16_t bend)
            : TrackEvent(timeSinceLastEvent)
            , m_bend(bend) {}

        std::int16_t getBend() const { return m_bend; }

        double getNormalizedBend() const;

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        std::int16_t m_bend;
    };

    struct MUSICLIB_API ChannelPressureEvent : public TrackEvent {
        DOWNCASTABLE(ChannelPressureEvent, TrackEvent);
        STREAM_EVENT(ChannelPressureEvent);

        ChannelPressureEvent(ModelDuration timeSinceLastEvent, Velocity value)
            : TrackEvent(timeSinceLastEvent)
            , m_value(value) {}

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Velocity m_value;
    };

    struct MUSICLIB_API PolyphonicAftertouchEvent : public TrackEvent {
        DOWNCASTABLE(PolyphonicAftertouchEvent, TrackEvent);
        STREAM_EVENT(PolyphonicAftertouchEvent);

        PolyphonicAftertouchEvent(ModelDuration timeSinceLastEvent, Pitch pitch, Velocity value)
            : TrackEvent(timeSinceLastEvent)
          , m_pitch(pitch)
          , m_value(value) {}

        Pitch getPitch() const { return m_pitch; }

        Velocity getMidiValue() const { return m_value; }

        double getNormalizedPressure() const { return static_cast<double>(m_value) / 127.0; }

        std::size_t getHash() const override;
        GroupingInfo getGroupingInfo() const override;

      protected:
        bool doIsEqualTo(const TrackEvent& other) const override;

      private:
        Pitch m_pitch;
        Velocity m_value;
    };

} // namespace bw_music