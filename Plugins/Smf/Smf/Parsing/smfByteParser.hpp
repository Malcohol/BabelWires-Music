/**
 * A steppable byte-level parser for Standard MIDI Files, which fires callbacks on a SequenceEventConsumer.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Smf/Parsing/smfTrackEventConsumer.hpp>
#include <Smf/Parsing/smfByteReader.hpp>

#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/Log/userLogger.hpp>
#include <BaseLib/Result/result.hpp>
#include <BaseLib/common.hpp>

#include <cstdint>
#include <memory>
#include <vector>

namespace smf {

    class SmfEventConsumer;

    /// The top-level driver for parsing a Standard MIDI File at the byte level.
    /// It reads the header chunk, buffers each track's bytes, and then steps through
    /// the events of all the tracks in global time order, firing callbacks on the
    /// SmfEventConsumer provided by the caller. Events at the same time are handled
    /// in track order (lower-numbered tracks first), so an event in one track can
    /// influence the consumer's interpretation of simultaneous or later events in
    /// other tracks.
    /// This class contains no Music-domain semantics: it decomposes the bytes into
    /// MIDI messages and reports them as events. Malformed or unsupported data
    /// is reported via the returned Result.
    class SmfByteParser : private SmfByteReader {
      public:
        /// The DataSource is consumed up to the end of the last track chunk. Any data
        /// beyond that is left unread.
        SmfByteParser(babelwires::DataSource& dataSource, SmfEventConsumer& consumer,
                      babelwires::UserAdvisoryLogger& log);

        /// Parse the entire file, firing callbacks on the consumer.
        babelwires::Result parse();

      private:
        babelwires::Result readHeaderChunk();

      private:
        SmfEventConsumer& m_consumer;
        babelwires::UserAdvisoryLogger& m_log;

        std::uint16_t m_numTracks = 0;
    };

} // namespace smf
