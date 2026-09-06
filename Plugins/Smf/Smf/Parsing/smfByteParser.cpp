/**
 * A steppable byte-level parser for Standard MIDI Files, which fires callbacks on a SequenceEventConsumer.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/Parsing/smfByteParser.hpp>

#include <BaseLib/Result/resultDSL.hpp>

#include <cassert>
#include <iomanip>
#include <utility>

// ---------------------------------------------------------------------------
// SmfByteReader
// ---------------------------------------------------------------------------

babelwires::ResultT<babelwires::Byte> smf::SmfByteReader::getNext() {
    const auto result = m_dataSource.getNextByte();
    if (!result) {
        return babelwires::Error() << "Stream is truncated (" << result.error().toString() << ")";
    }
    return result;
}

babelwires::ResultT<babelwires::Byte> smf::SmfByteReader::peekNext() {
    const auto result = m_dataSource.peekNextByte();
    if (!result) {
        return babelwires::Error() << "Stream is truncated (" << result.error().toString() << ")";
    }
    return result;
}

babelwires::Result smf::SmfByteReader::readByteSequence(const char* seq) {
    assert(seq);
    while (*seq) {
        ASSIGN_OR_ERROR(const babelwires::Byte c, getNext());
        if (c != *seq) {
            return babelwires::Error() << "Expected " << *seq << " at index " << m_dataSource.getAbsolutePosition()
                                       << " but found " << c << " instead";
        }
        ++seq;
    }
    return {};
}

babelwires::ResultT<std::uint8_t> smf::SmfByteReader::readU7() {
    ASSIGN_OR_ERROR(const babelwires::Byte b, getNext());
    if (b & 0x80) {
        return babelwires::Error() << "Expected a 7-bit MIDI data byte at index "
                                   << m_dataSource.getAbsolutePosition() << " but found "
                                   << static_cast<int>(b) << "; the data is likely corrupted";
    }
    return b;
}

babelwires::ResultT<std::uint16_t> smf::SmfByteReader::readU14() {
    ASSIGN_OR_ERROR(const std::uint16_t lsb, readU7());
    ASSIGN_OR_ERROR(const std::uint16_t msb, readU7());
    return (msb << 7) | lsb;
}

babelwires::ResultT<std::uint16_t> smf::SmfByteReader::readU16() {
    ASSIGN_OR_ERROR(const std::uint32_t b0, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b1, getNext());
    return (b0 << 8) | b1;
}

babelwires::ResultT<std::uint32_t> smf::SmfByteReader::readU24() {
    ASSIGN_OR_ERROR(const std::uint32_t b0, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b1, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b2, getNext());
    return (b0 << 16) | (b1 << 8) | b2;
}

babelwires::ResultT<std::uint32_t> smf::SmfByteReader::readU32() {
    ASSIGN_OR_ERROR(const std::uint32_t b0, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b1, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b2, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t b3, getNext());
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

babelwires::ResultT<std::uint32_t> smf::SmfByteReader::readVariableLengthQuantity() {
    std::uint32_t result = 0;
    babelwires::Byte b;
    int numBytes = 0;
    do {
        if (numBytes == 4) {
            return babelwires::Error() << "Variable Length Quantity too big";
        }
        ++numBytes;
        ASSIGN_OR_ERROR(b, getNext());
        result = (result << 7) + (b & 0x7f);
    } while (b & 0x80);
    return result;
}

babelwires::Result smf::SmfByteReader::skipBytes(std::uint32_t numBytes) {
    for (std::uint32_t i = 0; i < numBytes; ++i) {
        DO_OR_ERROR(getNext());
    }
    return {};
}

// ---------------------------------------------------------------------------
// SmfByteParser
// ---------------------------------------------------------------------------

smf::SmfByteParser::SmfByteParser(babelwires::DataSource& dataSource, SequenceEventConsumer& consumer,
                                  babelwires::UserAdvisoryLogger& log)
    : SmfByteReader(dataSource)
    , m_consumer(consumer)
    , m_log(log) {}

babelwires::Result smf::SmfByteParser::parse() {
    DO_OR_ERROR(readHeaderChunk());

    for (std::uint16_t i = 0; i < m_numTracks; ++i) {
        DO_OR_ERROR(readByteSequence("MTrk"));
        ASSIGN_OR_ERROR(const std::uint32_t trackLength, readU32());
        std::unique_ptr<TrackEventConsumer> trackConsumer = m_consumer.onTrack(i);
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
                                                         TrackEventConsumer& trackConsumer) {
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

// ---------------------------------------------------------------------------
// SmfTrackByteParser
// ---------------------------------------------------------------------------

smf::SmfTrackByteParser::SmfTrackByteParser(babelwires::DataSource& dataSource, TrackEventConsumer& consumer,
                                            std::uint32_t trackLength, std::uint16_t trackIndex,
                                            babelwires::UserAdvisoryLogger& log)
    : SmfByteReader(dataSource)
    , m_consumer(consumer)
    , m_log(log)
    , m_trackLength(trackLength)
    , m_trackDataStart(dataSource.getAbsolutePosition())
    , m_trackIndex(trackIndex) {}

babelwires::Result smf::SmfTrackByteParser::readMessageBytes(std::uint32_t length) {
    m_messageBuffer.resize(length);
    for (std::uint32_t i = 0; i < length; ++i) {
        ASSIGN_OR_ERROR(m_messageBuffer[i], getNext());
    }
    return {};
}

template <typename STREAMLIKE>
babelwires::Result smf::SmfTrackByteParser::logMessageBytes(STREAMLIKE log, std::uint32_t length) {
    DO_OR_ERROR(readMessageBytes(length));
    if (length > 0) {
        log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[0]);
        for (std::uint32_t i = 1; i < length; ++i) {
            log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[i]);
        }
    }
    return {};
}

template <typename... ARGS, typename... CALL_ARGS>
babelwires::Result smf::SmfTrackByteParser::fireCallback(
    std::uint64_t delta, babelwires::ResultT<EventHandlingResult> (TrackEventConsumer::*callback)(TimeInfo, ARGS...),
    CALL_ARGS&&... args) {
    m_ticksSinceLastHandledEvent += delta;
    m_ticksSinceTrackStart += delta;

    const TimeInfo timeInfo{m_ticksSinceLastHandledEvent, m_ticksSinceTrackStart};
    ASSIGN_OR_ERROR(const EventHandlingResult result,
                    (m_consumer.*callback)(timeInfo, std::forward<CALL_ARGS>(args)...));
    return handleCallbackResult(result);
}

babelwires::Result smf::SmfTrackByteParser::handleCallbackResult(EventHandlingResult result) {
    switch (result) {
        case EventHandlingResult::Handled:
            m_ticksSinceLastHandledEvent = 0;
            break;
        case EventHandlingResult::Done:
            m_state = State::Done;
            break;
        case EventHandlingResult::Ignored:
            break;
    }
    return {};
}

babelwires::Result smf::SmfTrackByteParser::parseNextEvent() {
    assert(m_state == State::Ready);
    if ((m_dataSource.getAbsolutePosition() - m_trackDataStart) >= static_cast<int>(m_trackLength)) {
        return babelwires::Error() << "Read all of track " << m_trackIndex
                                   << " without finding an end-of-track event";
    }

    // Read the delta time.
    ASSIGN_OR_ERROR(const std::uint32_t delta, readVariableLengthQuantity());

    // Peek in case running status should be used.
    ASSIGN_OR_ERROR(babelwires::Byte statusByte, peekNext());
    if ((statusByte & 0x80) || (m_lastStatusByte == 0)) {
        // A new status byte, so consume it.
        DO_OR_ERROR(getNext());

        // Buffer stores the status when a Voice Category Status (ie, 0x80 to 0xEF) is received.
        // Buffer is cleared when a System Common Category Status (ie, 0xF0 to 0xF7) is received.
        // Nothing is done to the buffer when a RealTime Category message is received.
        if ((statusByte >= 0x80) && (statusByte <= 0xEF)) {
            // Voice category status
            m_lastStatusByte = statusByte;
        } else if ((statusByte >= 0xF0) && (statusByte <= 0xF7)) {
            // System common category status
            m_lastStatusByte = 0;
        }
    } else {
        // Running status.
        statusByte = m_lastStatusByte;
    }

    const babelwires::Byte statusHi = statusByte >> 4;
    const babelwires::Byte statusLo = statusByte & 0xf;

    if (statusHi == 0b1111) {
        return parseSystemMessage(statusLo, delta);
    } else {
        return parseChannelMessage(statusHi, statusLo, delta);
    }
}

babelwires::Result smf::SmfTrackByteParser::parseChannelMessage(babelwires::Byte statusHi, babelwires::Byte statusLo,
                                                                std::uint64_t delta) {
    switch (statusHi) {
        case 0b1000: // Note off.
        {
            ASSIGN_OR_ERROR(const std::uint8_t pitch, readU7());
            ASSIGN_OR_ERROR(const std::uint8_t velocity, readU7());
            return fireCallback(delta, &TrackEventConsumer::onNoteOff, statusLo, pitch, velocity);
        }
        case 0b1001: // Note on.
        {
            ASSIGN_OR_ERROR(const std::uint8_t pitch, readU7());
            ASSIGN_OR_ERROR(const std::uint8_t velocity, readU7());
            if (velocity != 0) {
                return fireCallback(delta, &TrackEventConsumer::onNoteOn, statusLo, pitch, velocity);
            } else {
                // A note-on with velocity 0 is defined as a note-off.
                return fireCallback(delta, &TrackEventConsumer::onNoteOff, statusLo, pitch, velocity);
            }
        }
        case 0b1010: // Polyphonic key pressure (aftertouch).
        {
            ASSIGN_OR_ERROR(const std::uint8_t pitch, readU7());
            ASSIGN_OR_ERROR(const std::uint8_t pressure, readU7());
            return fireCallback(delta, &TrackEventConsumer::onPolyphonicAftertouch, statusLo, pitch, pressure);
        }
        case 0b1011: // Control change.
        {
            ASSIGN_OR_ERROR(const std::uint8_t controller, readU7());
            ASSIGN_OR_ERROR(const std::uint8_t value, readU7());
            if (controller >= 120) {
                // Channel mode message.
                TrackEventConsumer::ChannelMode mode;
                switch (controller) {
                    case 120: mode = TrackEventConsumer::ChannelMode::AllSoundOff; break;
                    case 121: mode = TrackEventConsumer::ChannelMode::ResetAllControllers; break;
                    case 122: mode = TrackEventConsumer::ChannelMode::LocalControl; break;
                    case 123: mode = TrackEventConsumer::ChannelMode::AllNotesOff; break;
                    case 124: mode = TrackEventConsumer::ChannelMode::OmniOff; break;
                    case 125: mode = TrackEventConsumer::ChannelMode::OmniOn; break;
                    case 126: mode = TrackEventConsumer::ChannelMode::Mono; break;
                    case 127: mode = TrackEventConsumer::ChannelMode::Poly; break;
                    default: assert(false && "Unexpected channel mode controller"); return {};
                }
                return fireCallback(delta, &TrackEventConsumer::onChannelMode, statusLo, mode, value);
            } else {
                return fireCallback(delta, &TrackEventConsumer::onControlChange, statusLo, controller, value);
            }
        }
        case 0b1100: // Program change.
        {
            ASSIGN_OR_ERROR(const std::uint8_t program, readU7());
            return fireCallback(delta, &TrackEventConsumer::onProgramChange, statusLo, program);
        }
        case 0b1101: // Channel pressure.
        {
            ASSIGN_OR_ERROR(const std::uint8_t pressure, readU7());
            return fireCallback(delta, &TrackEventConsumer::onChannelPressure, statusLo, pressure);
        }
        case 0b1110: // Pitch wheel.
        {
            ASSIGN_OR_ERROR(const std::uint16_t value14, readU14());
            return fireCallback(delta, &TrackEventConsumer::onPitchBend, statusLo, value14);
        }
        default: {
            return babelwires::Error() << "Unrecognized MIDI message with status hi-nibble "
                                       << static_cast<int>(statusHi);
        }
    }
}

babelwires::Result smf::SmfTrackByteParser::parseSystemMessage(babelwires::Byte statusLo, std::uint64_t delta) {
    if (statusLo == 0x00) {
        // SysEx event (0xF0).
        ASSIGN_OR_ERROR(const std::uint32_t length, readVariableLengthQuantity());
        if (length < 1) {
            return babelwires::Error() << "SysEx event in track " << m_trackIndex << " has invalid length";
        }
        DO_OR_ERROR(readMessageBytes(length));
        return fireCallback(delta, &TrackEventConsumer::onSysExEvent,
                            std::span<const std::uint8_t>(m_messageBuffer.data(), m_messageBuffer.size()));
    } else if (statusLo == 0x07) {
        // SysEx continuation (0xF7).
        ASSIGN_OR_ERROR(const std::uint32_t length, readVariableLengthQuantity());
        DO_OR_ERROR(readMessageBytes(length));
        return fireCallback(delta, &TrackEventConsumer::onSysExContinuationEvent,
                            std::span<const std::uint8_t>(m_messageBuffer.data(), m_messageBuffer.size()));
    } else if (statusLo == 0x01) {
        // MTC Quarter Frame (0xF1).
        ASSIGN_OR_ERROR(const std::uint8_t value, readU7());
        return fireCallback(delta, &TrackEventConsumer::onMtcQuarterFrame, value);
    } else if (statusLo == 0x02) {
        // Song Position Pointer (0xF2).
        ASSIGN_OR_ERROR(const std::uint16_t value14, readU14());
        return fireCallback(delta, &TrackEventConsumer::onSongPositionPointer, value14);
    } else if (statusLo == 0x03) {
        // Song Select (0xF3).
        ASSIGN_OR_ERROR(const std::uint8_t song, readU7());
        return fireCallback(delta, &TrackEventConsumer::onSongSelect, song);
    } else if (statusLo == 0x06) {
        // Tune Request (0xF6).
        return fireCallback(delta, &TrackEventConsumer::onTuneRequest);
    } else if (statusLo == 0x0f) {
        // Meta-event (0xFF).
        return parseMetaEvent(delta);
    } else {
        return babelwires::Error() << "Unrecognized MIDI message with status byte "
                                   << static_cast<int>(0xF0 | statusLo) << " in track " << m_trackIndex;
    }
}

babelwires::Result smf::SmfTrackByteParser::parseMetaEvent(std::uint64_t delta) {
    ASSIGN_OR_ERROR(const babelwires::Byte type, getNext());
    ASSIGN_OR_ERROR(const std::uint32_t length, readVariableLengthQuantity());

    switch (type) {
        case 0x00: // Sequence number
        {
            if (length != 2) {
                return babelwires::Error() << "Sequence number meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const std::uint16_t seqNum, readU16());
            return fireCallback(delta, &TrackEventConsumer::onSequenceNumber, seqNum);
        }
        case 0x01: // Text event
        case 0x02: // Copyright
        case 0x03: // Sequence or track name
        case 0x04: // Instrument name
        case 0x05: // Lyric
        case 0x06: // Marker
        case 0x07: // Cue point
        case 0x08: // Program name
        case 0x09: // Device name
        {
            DO_OR_ERROR(readMessageBytes(length));
            const std::span<const std::uint8_t> text(m_messageBuffer.data(), m_messageBuffer.size());
            switch (type) {
                case 0x01:
                    return fireCallback(delta, &TrackEventConsumer::onTextEvent, text);
                case 0x02:
                    return fireCallback(delta, &TrackEventConsumer::onCopyright, text);
                case 0x03:
                    return fireCallback(delta, &TrackEventConsumer::onSequenceOrTrackName, text);
                case 0x04:
                    return fireCallback(delta, &TrackEventConsumer::onInstrumentName, text);
                case 0x05:
                    return fireCallback(delta, &TrackEventConsumer::onLyric, text);
                case 0x06:
                    return fireCallback(delta, &TrackEventConsumer::onMarker, text);
                case 0x07:
                    return fireCallback(delta, &TrackEventConsumer::onCuePoint, text);
                case 0x08:
                    return fireCallback(delta, &TrackEventConsumer::onProgramName, text);
                case 0x09:
                    return fireCallback(delta, &TrackEventConsumer::onDeviceName, text);
                default:
                    assert(false && "Unexpected text meta-event type");
                    return {};
            }
        }
        case 0x20: // Channel prefix
        {
            if (length != 1) {
                return babelwires::Error() << "Channel prefix meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const std::uint8_t channel, readU7());
            return fireCallback(delta, &TrackEventConsumer::onChannelPrefix, channel);
        }
        case 0x21: // MIDI port
        {
            if (length != 1) {
                return babelwires::Error() << "MIDI port meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const std::uint8_t port, readU7());
            return fireCallback(delta, &TrackEventConsumer::onMidiPort, port);
        }
        case 0x2F: // End of track.
        {
            if ((m_dataSource.getAbsolutePosition() - m_trackDataStart) != static_cast<int>(m_trackLength)) {
                return babelwires::Error() << "MIDI track " << m_trackIndex << " had an unexpected end-of-track event";
            }
            if (length != 0) {
                return babelwires::Error() << "End of Track meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            // Fire the callback, then mark as done.
            DO_OR_ERROR(fireCallback(delta, &TrackEventConsumer::onEndOfTrack));
            m_state = State::Done;
            return {};
        }
        case 0x51: // Set tempo
        {
            if (length != 3) {
                return babelwires::Error() << "Tempo meta-event in track " << m_trackIndex << " has incorrect length "
                                           << length;
            }
            ASSIGN_OR_ERROR(const std::uint32_t tempoValue, readU24());
            return fireCallback(delta, &TrackEventConsumer::onTempoEvent, tempoValue);
        }
        case 0x54: // SMPTE offset
        {
            if (length != 5) {
                return babelwires::Error() << "SMPTE Offset meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const babelwires::Byte hr, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte mn, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte se, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte fr, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte ff, getNext());

            TrackEventConsumer::SmpteFrameRate frameRate;
            switch (hr >> 5) {
                case 0: frameRate = TrackEventConsumer::SmpteFrameRate::Fps24; break;
                case 1: frameRate = TrackEventConsumer::SmpteFrameRate::Fps25; break;
                case 2: frameRate = TrackEventConsumer::SmpteFrameRate::Fps29Drop; break;
                case 3: frameRate = TrackEventConsumer::SmpteFrameRate::Fps30; break;
                default: assert(false && "Unexpected SMPTE frame rate"); return {};
            }
            const TrackEventConsumer::SmpteOffset smpteOffset{frameRate, static_cast<std::uint8_t>(hr & 0x1f),
                                                              mn, se, fr, ff};
            return fireCallback(delta, &TrackEventConsumer::onSmpteOffset, smpteOffset);
        }
        case 0x58: // Time signature
        {
            if (length != 4) {
                return babelwires::Error() << "Time Signature meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const babelwires::Byte nn, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte dd, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte cc, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte bb, getNext());
            return fireCallback(delta, &TrackEventConsumer::onTimeSignature, static_cast<std::int8_t>(nn),
                                static_cast<std::int8_t>(dd), static_cast<std::int8_t>(cc),
                                static_cast<std::int8_t>(bb));
        }
        case 0x59: // Key signature
        {
            if (length != 2) {
                return babelwires::Error() << "Key Signature meta-event in track " << m_trackIndex
                                           << " has incorrect length " << length;
            }
            ASSIGN_OR_ERROR(const babelwires::Byte sf, getNext());
            ASSIGN_OR_ERROR(const babelwires::Byte mi, getNext());
            if (mi > 1) {
                return babelwires::Error() << "Key Signature meta-event in track " << m_trackIndex
                                           << " has invalid mode " << static_cast<int>(mi);
            }
            const auto mode = (mi == 0) ? TrackEventConsumer::KeySignatureMode::Major
                                        : TrackEventConsumer::KeySignatureMode::Minor;
            return fireCallback(delta, &TrackEventConsumer::onKeySignature, sf, mode);
        }
        case 0x7F: // Sequencer specific event
        {
            DO_OR_ERROR(readMessageBytes(length));
            return fireCallback(delta, &TrackEventConsumer::onSequencerSpecificEvent,
                                std::span<const std::uint8_t>(m_messageBuffer.data(), m_messageBuffer.size()));
        }
        default: // Unknown meta-event type
        {
            // This event can be parsed, but it is outside the spec and has no callback.
            // Log a warning and skip it.
            DO_OR_ERROR(logMessageBytes(m_log.logWarning()
                                            << "Skipping unknown meta-event of type " << std::hex
                                            << static_cast<int>(type) << " in track " << m_trackIndex << ": ",
                                        length));
            // Accumulate the delta time since no event was delivered.
            m_ticksSinceLastHandledEvent += delta;
            m_ticksSinceTrackStart += delta;
            return {};
        }
    }
}
