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
        SmfByteParser(babelwires::DataSource& dataSource, SmfEventConsumer& consumer,
                      babelwires::UserAdvisoryLogger& log);

        /// Parse the entire file, firing callbacks on the consumer.
        babelwires::Result parse();

      private:
        babelwires::Result readHeaderChunk();
        babelwires::Result readTrackContents(std::uint16_t trackIndex, std::uint32_t trackLength, SmfTrackEventConsumer& trackConsumer);

      private:
        SmfEventConsumer& m_consumer;
        babelwires::UserAdvisoryLogger& m_log;

        std::uint16_t m_numTracks = 0;
    };

} // namespace smf
