/**
 * A steppable byte-level parser for Standard MIDI Files, which fires callbacks on a SequenceEventConsumer.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Smf/Parsing/eventConsumers.hpp>

#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/Log/userLogger.hpp>
#include <BaseLib/Result/result.hpp>
#include <BaseLib/common.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace smf {

    /// Low-level big-endian byte reading over a DataSource, shared by the
    /// byte-level SMF parsers. All multi-byte reads are big-endian (network order),
    /// as required by the SMF format.
    class SmfByteReader {
      public:
        SmfByteReader(babelwires::DataSource& dataSource)
            : m_dataSource(dataSource) {}

        babelwires::ResultT<babelwires::Byte> getNext();
        babelwires::ResultT<babelwires::Byte> peekNext();

        /// Read a MIDI data byte, which the standard mandates is 7-bit.
        /// Returns an error if the byte has its high bit set.
        babelwires::ResultT<std::uint8_t> readU7();

        /// Read a 14-bit value encoded as two 7-bit MIDI data bytes (LSB then MSB).
        babelwires::ResultT<std::uint16_t> readU14();

        /// Read the expected byte sequence.
        babelwires::Result readByteSequence(const char* seq);

        babelwires::ResultT<std::uint16_t> readU16();
        babelwires::ResultT<std::uint32_t> readU24();
        babelwires::ResultT<std::uint32_t> readU32();
        babelwires::ResultT<std::uint32_t> readVariableLengthQuantity();

        /// Consume and discard numBytes.
        babelwires::Result skipBytes(std::uint32_t numBytes);

      protected:
        babelwires::DataSource& m_dataSource;
    };

    /// The top-level driver for parsing a Standard MIDI File at the byte level.
    /// It reads the header chunk and then steps through each track in file order,
    /// firing callbacks on the SequenceEventConsumer provided by the caller.
    /// This class contains no Music-domain semantics: it decomposes the bytes into
    /// MIDI messages and reports them as events. Malformed or unsupported data
    /// is reported via the returned Result.
    /// Note: Tracks are processed sequentially in file order, not in global time
    /// order. A future enhancement could use a priority queue over per-track
    /// SmfTrackByteParser instances to deliver events in global time order.
    class SmfByteParser : private SmfByteReader {
      public:
        /// The DataSource is not guaranteed to be fully consumed, even in the successful case:
        /// a track consumer which returns Done causes the remainder of that track to be skipped
        /// without parsing.
        SmfByteParser(babelwires::DataSource& dataSource, SequenceEventConsumer& consumer,
                      babelwires::UserAdvisoryLogger& log);

        /// Parse the entire file, firing callbacks on the consumer.
        babelwires::Result parse();

      private:
        babelwires::Result readHeaderChunk();
        babelwires::Result readTrackContents(std::uint16_t trackIndex, std::uint32_t trackLength, TrackEventConsumer& trackConsumer);

      private:
        SequenceEventConsumer& m_consumer;
        babelwires::UserAdvisoryLogger& m_log;

        std::uint16_t m_numTracks = 0;
    };

    /// Parses the bytes of a single MIDI track chunk, firing callbacks on a
    /// TrackEventConsumer. This is designed to be steppable: parseNextEvent()
    /// processes exactly one event, so a future merged driver can interleave
    /// multiple tracks in global time order by stepping each track's parser.
    class SmfTrackByteParser : private SmfByteReader {
      public:
        /// The DataSource must be positioned at the first byte of the track's event data
        /// (i.e. after the "MTrk" ID and length field).
        SmfTrackByteParser(babelwires::DataSource& dataSource, TrackEventConsumer& consumer, std::uint32_t trackLength,
                           std::uint16_t trackIndex, babelwires::UserAdvisoryLogger& log);

        enum class State {
            /// Ready to parse the next event.
            Ready,
            /// The end-of-track event has been parsed and the consumer has been notified.
            Done
        };

        State getState() const { return m_state; }

        /// MIDI ticks since the start of the track, up to and including the most
        /// recently parsed event. When getState() == Ready, this is the time of the
        /// next event minus its (unparsed) delta time.
        std::uint64_t getTicksSinceTrackStart() const { return m_ticksSinceTrackStart; }

        /// The number of bytes of track event data consumed so far.
        std::uint32_t getNumBytesConsumed() const {
            return static_cast<std::uint32_t>(m_dataSource.getAbsolutePosition() - m_trackDataStart);
        }

        /// Parse the next event in the track and fire the appropriate callback.
        /// Requires getState() == Ready.
        babelwires::Result parseNextEvent();

      private:
        using EventHandlingResult = TrackEventConsumer::EventHandlingResult;
        using TimeInfo = TrackEventConsumer::TimeInfo;

        /// Advance the running time totals by the given delta, then fire the consumer callback
        /// identified by the member function pointer, forwarding the given arguments.
        /// If the callback returns ResetTime, m_ticksSinceLastHandledEvent is reset.
        /// ARGS are the argument types of the callback (which may include const-ref or span);
        /// CALL_ARGS are the (deduced) argument types at the call site.
        template <typename... ARGS, typename... CALL_ARGS>
        babelwires::Result fireCallback(
            std::uint64_t delta,
            babelwires::ResultT<EventHandlingResult> (TrackEventConsumer::*callback)(TimeInfo, ARGS...),
            CALL_ARGS&&... args);

        /// Handle the result of firing a callback: update time accumulators and
        /// detect the Done signal.
        babelwires::Result handleCallbackResult(EventHandlingResult result);

        babelwires::Result parseChannelMessage(babelwires::Byte statusHi, babelwires::Byte statusLo,
                                               std::uint64_t delta);
        babelwires::Result parseSystemMessage(babelwires::Byte statusLo, std::uint64_t delta);
        babelwires::Result parseMetaEvent(std::uint64_t delta);

        /// Read length bytes into m_messageBuffer.
        babelwires::Result readMessageBytes(std::uint32_t length);

        /// Log the current contents of m_messageBuffer as hex bytes.
        /// Used for parseable events which are outside the spec and have no callback.
        template <typename STREAMLIKE> babelwires::Result logMessageBytes(STREAMLIKE log, std::uint32_t length);

      private:
        TrackEventConsumer& m_consumer;
        babelwires::UserAdvisoryLogger& m_log;

        /// The declared length of the track chunk in bytes.
        std::uint32_t m_trackLength;
        /// The absolute position in the data source of the first byte of track event data.
        int m_trackDataStart;
        /// The track index, used in error messages.
        std::uint16_t m_trackIndex;

        State m_state = State::Ready;

        /// MIDI ticks since the last event that was handled by the consumer.
        std::uint64_t m_ticksSinceLastHandledEvent = 0;
        /// MIDI ticks since the start of the track.
        std::uint64_t m_ticksSinceTrackStart = 0;

        /// The running status byte, or 0 if there is none.
        babelwires::Byte m_lastStatusByte = 0;

        /// Scratch buffer for SysEx and other variable-length message data.
        std::vector<babelwires::Byte> m_messageBuffer;
    };

} // namespace smf
