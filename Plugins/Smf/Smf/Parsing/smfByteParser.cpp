/**
 * A steppable byte-level parser for Standard MIDI Files, which fires callbacks on a SequenceEventConsumer.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/Parsing/smfByteParser.hpp>

#include <Smf/Parsing/smfTrackByteParser.hpp>
#include <Smf/Parsing/smfEventConsumer.hpp>

#include <BaseLib/IO/bufferDataSource.hpp>
#include <BaseLib/Result/resultDSL.hpp>

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <utility>

smf::SmfByteParser::SmfByteParser(babelwires::DataSource& dataSource, SmfEventConsumer& consumer,
                                  babelwires::UserAdvisoryLogger& log)
    : SmfByteReader(dataSource)
    , m_consumer(consumer)
    , m_log(log) {}

babelwires::Result smf::SmfByteParser::parse() {
    DO_OR_ERROR(readHeaderChunk());

    // A track which the consumer is interested in, with the state needed to step its parser.
    struct ActiveTrack {
        // Member order matters: the parser holds references to the source and the consumer,
        // so it must be destroyed first.
        std::unique_ptr<SmfTrackEventConsumer> m_consumer;
        std::unique_ptr<babelwires::BufferDataSource> m_dataSource;
        std::unique_ptr<SmfTrackByteParser> m_parser;
        std::uint64_t m_ticksOfNextEvent = 0;
    };

    std::vector<ActiveTrack> activeTracks;

    // Buffer each track's bytes into its own data source, so the track parsers can be
    // stepped independently of one another.
    for (std::uint16_t i = 0; i < m_numTracks; ++i) {
        DO_OR_ERROR(readByteSequence("MTrk"));
        ASSIGN_OR_ERROR(const std::uint32_t trackLength, readU32());
        std::unique_ptr<SmfTrackEventConsumer> trackConsumer = m_consumer.onTrack(i);
        if (trackConsumer) {
            ASSIGN_OR_ERROR(std::vector<babelwires::Byte> trackBytes, m_dataSource.readBytes(trackLength));
            ActiveTrack track;
            track.m_dataSource = std::make_unique<babelwires::BufferDataSource>(std::move(trackBytes));
            track.m_parser =
                std::make_unique<SmfTrackByteParser>(*track.m_dataSource, *trackConsumer, trackLength, i, m_log);
            ASSIGN_OR_ERROR(track.m_ticksOfNextEvent, track.m_parser->getTicksOfNextEvent());
            track.m_consumer = std::move(trackConsumer);
            activeTracks.emplace_back(std::move(track));
        } else {
            // The consumer is not interested in this track: skip its bytes.
            DO_OR_ERROR(skipBytes(trackLength));
        }
    }

    // Step the track parsers in global time order, so an event in one track can affect the
    // consumer's interpretation of later events in other tracks. Ties are broken by track
    // order in the file (min_element yields the first minimizer, and activeTracks is ordered
    // by track index), so events in lower-numbered tracks are handled first.
    while (!activeTracks.empty()) {
        const auto nextIt = std::min_element(activeTracks.begin(), activeTracks.end(),
                                             [](const ActiveTrack& a, const ActiveTrack& b) {
                                                 return a.m_ticksOfNextEvent < b.m_ticksOfNextEvent;
                                             });
        ActiveTrack& track = *nextIt;
        DO_OR_ERROR(track.m_parser->parseNextEvent());
        if (track.m_parser->getState() == SmfTrackByteParser::State::Done) {
            // The track is finished. Destroying the consumer lets it complete any per-track work.
            activeTracks.erase(nextIt);
        } else {
            ASSIGN_OR_ERROR(track.m_ticksOfNextEvent, track.m_parser->getTicksOfNextEvent());
        }
    }
    return {};
}

babelwires::Result smf::SmfByteParser::readHeaderChunk() {
    DO_OR_ERROR(readByteSequence("MThd"));
    {
        ASSIGN_OR_ERROR(const auto headerLength, readU32());
        if (headerLength != 6) {
            return babelwires::Error() << "Header chunks are expected to be 6 bytes long, not " << headerLength;
        }
    }
    ASSIGN_OR_ERROR(const std::uint16_t format, readU16());
    if (format > 2) {
        return babelwires::Error() << "Format " << format << " is not a known type of Standard MIDI File";
    }
    ASSIGN_OR_ERROR(m_numTracks, readU16());
    // Note: This parser has no opinion on SMPTE (division bit 15 set). It's up to the consumer to judge
    // how to interpret the data, given the division value.
    ASSIGN_OR_ERROR(const std::uint16_t division, readU16());

    return m_consumer.onSequenceStart(m_numTracks, format, division);
}
