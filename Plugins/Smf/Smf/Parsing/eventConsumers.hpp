/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BaseLib/Result/result.hpp>

#include <cstdint>
#include <memory>
#include <span>

namespace smf {
    class SmfTrackEventConsumer {
      public:
        virtual ~SmfTrackEventConsumer() = default;

        enum class EventHandlingResult {
            /// The event should be treated as "handled" by the event consumer, so the time since the last handled event
            /// should be reset.
            ResetTime,
            /// The event should be treated as "not handled" by the event consumer, so the time since the last handled
            /// event should be accumulated. Because the default implementation of callbacks return `AccumulateTime`,
            /// subclasses can simply ignore events they are not interested in.
            AccumulateTime,
            /// Stop sending events to this consumer.
            Done
        };

        struct TimeInfo {
            /// MIDI ticks since the last event that was handled by the event consumer.
            std::uint64_t m_ticksSinceLastHandledEvent;
            /// MIDI ticks since the start of the track.
            std::uint64_t m_ticksSinceTrackStart;
        };

        /// Note On.
        /// A Note On message with velocity 0 is defined by the MIDI spec as a Note Off. Concrete parsers are
        /// expected to perform this translation and deliver such messages via onNoteOff, so this callback is only
        /// issued for Note On messages with non-zero velocity.
        virtual babelwires::ResultT<EventHandlingResult> onNoteOn(TimeInfo timeInfo, std::uint8_t channel4,
                                                                  std::uint8_t pitch7, std::uint8_t velocity7) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Note Off. Also issued for Note On messages with velocity 0, which the MIDI spec defines as Note Off.
        virtual babelwires::ResultT<EventHandlingResult> onNoteOff(TimeInfo timeInfo, std::uint8_t channel4,
                                                                   std::uint8_t pitch7, std::uint8_t velocity7) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult>
        onPolyphonicAftertouch(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t pitch7, std::uint8_t pressure7) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onChannelPressure(TimeInfo timeInfo, std::uint8_t channel4,
                                                                           std::uint8_t pressure7) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onPitchBend(TimeInfo timeInfo, std::uint8_t channel4,
                                                                     std::uint16_t value14) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Generic control change message. This is issued for all control change messages, except channel mode
        /// messages, which have their own callback below.
        /// The 14-bit controllers (e.g. volume, pan, expression, bank select) are delivered as separate MSB and LSB
        /// control change messages; pairing them is left to the consumer.
        /// TODO: RPN/NRPN messages (controllers 96-101 with data entry controllers 6 and 38) are delivered via this
        /// callback. Assembling them into parameter changes is left to consumers for now, but could be handled by
        /// the concrete parser in the future.
        virtual babelwires::ResultT<EventHandlingResult>
        onControlChange(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t controller7, std::uint8_t value7) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Channel mode messages (control change controllers 120-127).
        enum class ChannelMode {
            AllSoundOff,         ///< Controller 120
            ResetAllControllers, ///< Controller 121
            LocalControl,        ///< Controller 122. Value is 0 (off) or 127 (on).
            AllNotesOff,         ///< Controller 123
            OmniOff,             ///< Controller 124
            OmniOn,              ///< Controller 125
            Mono,                ///< Controller 126. Value is the number of channels.
            Poly                 ///< Controller 127
        };

        /// The value7 is only meaningful for LocalControl and Mono.
        virtual babelwires::ResultT<EventHandlingResult> onChannelMode(TimeInfo timeInfo, std::uint8_t channel4,
                                                                       ChannelMode mode, std::uint8_t value7) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onProgramChange(TimeInfo timeInfo, std::uint8_t channel4,
                                                                         std::uint8_t program7) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onTempoEvent(TimeInfo timeInfo, std::uint32_t tempoValue24) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onTimeSignature(TimeInfo timeInfo, std::int8_t numerator,
                                                                         std::int8_t denominatorPower,
                                                                         std::int8_t midiClocksPerMetronomeClick,
                                                                         std::int8_t num32ndNotesPerMidiQuarterNote) {
            return EventHandlingResult::AccumulateTime;
        }

        enum class KeySignatureMode { Major, Minor };

        virtual babelwires::ResultT<EventHandlingResult>
        onKeySignature(TimeInfo timeInfo, std::uint8_t m_numSharpsOrFlats, KeySignatureMode mode) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onSequenceOrTrackName(TimeInfo timeInfo,
                                                                               std::span<const std::uint8_t> name) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onInstrumentName(TimeInfo timeInfo,
                                                                          std::span<const std::uint8_t> name) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onTextEvent(TimeInfo timeInfo,
                                                                     std::span<const std::uint8_t> text) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onCopyright(TimeInfo timeInfo,
                                                                     std::span<const std::uint8_t> copyright) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onLyric(TimeInfo timeInfo,
                                                                 std::span<const std::uint8_t> lyric) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onMarker(TimeInfo timeInfo,
                                                                  std::span<const std::uint8_t> marker) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onCuePoint(TimeInfo timeInfo,
                                                                    std::span<const std::uint8_t> cuePoint) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onProgramName(TimeInfo timeInfo,
                                                                       std::span<const std::uint8_t> name) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onDeviceName(TimeInfo timeInfo,
                                                                      std::span<const std::uint8_t> name) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onSequenceNumber(TimeInfo timeInfo,
                                                                          std::uint16_t sequenceNumber16) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onChannelPrefix(TimeInfo timeInfo, std::uint8_t channel4) {
            return EventHandlingResult::AccumulateTime;
        }

        /// MIDI Port meta-event: routes the track to an output port, superseding the channel prefix.
        virtual babelwires::ResultT<EventHandlingResult> onMidiPort(TimeInfo timeInfo, std::uint8_t port7) {
            return EventHandlingResult::AccumulateTime;
        }

        enum class SmpteFrameRate { Fps24, Fps25, Fps29Drop, Fps30 };

        struct SmpteOffset {
            SmpteFrameRate m_frameRate;
            std::uint8_t m_hours;
            std::uint8_t m_minutes;
            std::uint8_t m_seconds;
            std::uint8_t m_frames;
            std::uint8_t m_fractionalFrames100;
        };

        virtual babelwires::ResultT<EventHandlingResult> onSmpteOffset(TimeInfo timeInfo,
                                                                       const SmpteOffset& smpteOffset) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onSysExEvent(TimeInfo timeInfo,
                                                                      std::span<const std::uint8_t> data) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onSysExContinuationEvent(TimeInfo timeInfo,
                                                                                  std::span<const std::uint8_t> data) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onSequencerSpecificEvent(TimeInfo timeInfo,
                                                                                  std::span<const std::uint8_t> data) {
            return EventHandlingResult::AccumulateTime;
        }

        /// MTC Quarter Frame (system common message 0xF1), which may legally appear in an SMF track.
        virtual babelwires::ResultT<EventHandlingResult> onMtcQuarterFrame(TimeInfo timeInfo, std::uint8_t value7) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Song Position Pointer (system common message 0xF2), which may legally appear in an SMF track.
        /// The value is in MIDI beats (16th notes) since the start of the song.
        virtual babelwires::ResultT<EventHandlingResult> onSongPositionPointer(TimeInfo timeInfo,
                                                                               std::uint16_t value14) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Song Select (system common message 0xF3), which may legally appear in an SMF track.
        virtual babelwires::ResultT<EventHandlingResult> onSongSelect(TimeInfo timeInfo, std::uint8_t song7) {
            return EventHandlingResult::AccumulateTime;
        }

        /// Tune Request (system common message 0xF6), which may legally appear in an SMF track.
        virtual babelwires::ResultT<EventHandlingResult> onTuneRequest(TimeInfo timeInfo) {
            return EventHandlingResult::AccumulateTime;
        }

        virtual babelwires::ResultT<EventHandlingResult> onEndOfTrack(TimeInfo timeInfo) {
            return EventHandlingResult::AccumulateTime;
        }
    };

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
