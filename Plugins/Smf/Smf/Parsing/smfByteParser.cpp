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

#include <BaseLib/Result/resultDSL.hpp>

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

    for (std::uint16_t i = 0; i < m_numTracks; ++i) {
        DO_OR_ERROR(readByteSequence("MTrk"));
        ASSIGN_OR_ERROR(const std::uint32_t trackLength, readU32());
        std::unique_ptr<SmfTrackEventConsumer> trackConsumer = m_consumer.onTrack(i);
        if (trackConsumer) {
            DO_OR_ERROR(readTrackContents(i, trackLength, *trackConsumer));
        } else {
            // The consumer is not interested in this track: skip its bytes.
            DO_OR_ERROR(skipBytes(trackLength));
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

babelwires::Result smf::SmfByteParser::readTrackContents(std::uint16_t trackIndex, std::uint32_t trackLength,
                                                         SmfTrackEventConsumer& trackConsumer) {
    SmfTrackByteParser trackParser(m_dataSource, trackConsumer, trackLength, trackIndex, m_log);
    while (trackParser.getState() == SmfTrackByteParser::State::Ready) {
        DO_OR_ERROR(trackParser.parseNextEvent());
    }
    // A consumer may signal Done before the end-of-track event, leaving part of the
    // track unparsed. Skip any remaining bytes so the next track starts at the right place.
    const std::uint32_t numBytesConsumed = trackParser.getNumBytesConsumed();
    if (numBytesConsumed < trackLength) {
        DO_OR_ERROR(skipBytes(trackLength - numBytesConsumed));
    }
    return {};
}
