/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/smfParser.hpp>

#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>

#include <BaseLib/Log/debugLogger.hpp>
#include <BaseLib/exceptions.hpp>

#include <cassert>
#include <cmath>
#include <iomanip>

namespace {
    static const int MAX_CHANNELS = 16;

    // See page 237 of the SC-8850 English manual
    const std::array<unsigned int, 16> s_gsBlockToPartMapping{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16};
} // namespace

smf::SmfParser::SmfParser(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext,
                          babelwires::UserLogger& userLogger)
    : m_projectContext(projectContext)
    , m_dataSource(dataSource)
    , m_userLogger(userLogger)
    , m_sequenceType(Format::SMF_UNKNOWN_FORMAT)
    , m_numTracks(-1)
    , m_division(-1)
    , m_standardPercussionSets(projectContext) {

    m_result = std::make_unique<babelwires::ValueTreeRoot>(projectContext.m_typeSystem, babelwires::FileTypeT<SmfSequence>::getThisIdentifier());
    m_result->setToDefault();
}

smf::SmfParser::~SmfParser() = default;

smf::SmfSequence::ConstInstance smf::SmfParser::getSmfSequenceConst() const {
    return babelwires::FileTypeT<SmfSequence>::ConstInstance(*m_result).getConts();
}

smf::SmfSequence::Instance smf::SmfParser::getSmfSequence() {
    return babelwires::FileTypeT<SmfSequence>::Instance(*m_result).getConts();
}

smf::SmfSequence::Instance getSmfSequence();


babelwires::Byte smf::SmfParser::getNext() {
    try {
        return m_dataSource.getNextByte();
    } catch (std::exception&) {
        throw babelwires::ParseException() << "Stream is truncated";
    }
}

babelwires::Byte smf::SmfParser::peekNext() {
    try {
        return m_dataSource.peekNextByte();
    } catch (std::exception&) {
        throw babelwires::ParseException() << "Stream is truncated";
    }
}

void smf::SmfParser::readByteSequence(const char* seq) {
    assert(seq);
    while (*seq) {
        const babelwires::Byte c = getNext();
        if (c != *seq) {
            throw babelwires::ParseException()
                << "Expected " << *seq << " at index " << m_dataSource.getAbsolutePosition() << " but found " << c
                << " instead";
        }
        ++seq;
    }
}

void smf::SmfParser::skipBytes(int numBytes) {
    for (int i = 0; i < numBytes; ++i) {
        getNext();
    }
}

std::uint16_t smf::SmfParser::readU16() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    return (b0 << 8) | b1;
}

std::uint32_t smf::SmfParser::readU24() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    const std::uint32_t b2 = getNext();
    return (b0 << 16) | (b1 << 8) | b2;
}

std::uint32_t smf::SmfParser::readU32() {
    const std::uint32_t b0 = getNext();
    const std::uint32_t b1 = getNext();
    const std::uint32_t b2 = getNext();
    const std::uint32_t b3 = getNext();
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

std::uint32_t smf::SmfParser::readVariableLengthQuantity() {
    std::uint32_t result = 0;
    babelwires::Byte b;
    int numBytes = 0;
    do {
        if (numBytes == 4) {
            throw babelwires::ParseException() << "Variable Length Quantity too big";
        }
        ++numBytes;

        b = getNext();
        result = (result << 7) + (b & 0x7f);
    } while (b & 0x80);
    return result;
}

bw_music::ModelDuration smf::SmfParser::readModelDuration() {
    const int numDivisions = readVariableLengthQuantity();
    return bw_music::ModelDuration(numDivisions) * bw_music::ModelDuration(1, m_division * 4);
}

std::string smf::SmfParser::readTextMetaEvent(int length) {
    std::vector<char> text;
    for (int i = 0; i < length; ++i) {
        text.push_back(getNext());
    }
    return std::string(text.data(), length);
}

void smf::SmfParser::readHeaderChunk() {
    readByteSequence("MThd");
    if (readU32() != 6) {
        throw babelwires::ParseException() << "Header chunk not expected length";
    }
    switch (readU16()) {
        case 0:
            m_sequenceType = Format::SMF_FORMAT_0;
            break;
        case 1:
            m_sequenceType = Format::SMF_FORMAT_1;
            break;
        case 2:
            throw babelwires::ParseException() << "Standard MIDI File Format 2 files are not currently supported";
        default:
            throw babelwires::ParseException() << "Not a known type of Standard MIDI File";
    }
    m_numTracks = readU16();
    m_division = readU16();
    if (m_division & (1 << 15)) {
        throw babelwires::ParseException() << "SMPTE format durations not supported";
    }
}

void smf::SmfParser::parse() {
    auto smfSequence = getSmfSequence();

    readHeaderChunk();
    switch (m_sequenceType) {
        case Format::SMF_FORMAT_0: {
            readFormat0Sequence();
            break;
        }
        case Format::SMF_FORMAT_1: {
            smfSequence.selectTag("SMF1");
            readFormat1Sequence();
            break;
        }
        case Format::SMF_FORMAT_2: {
            // TODO
        }
        case Format::SMF_UNKNOWN_FORMAT:
        default: {
            throw babelwires::ParseException() << "The data is not in one of the understood sequence types";
        }
    }
}

void smf::SmfParser::readTempoEvent(std::uint32_t tempoValue) {
    const double d = tempoValue;
    const double bpm = 60'000'000 / d;
    getMidiMetadata().activateAndGetTempo().set(std::round(bpm));
}

class smf::SmfParser::TrackSplitter {
  public:
    TrackSplitter(const std::array<ChannelSetup, 16>& channelSetup)
        : m_channels{}
        , m_channelSetup(channelSetup) {}

    bool addNoteOn(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent, bw_music::Pitch pitch,
                   bw_music::Velocity velocity) {
        if (const bw_music::PercussionSetWithPitchMap* const percussionSet =
                m_channelSetup[channelNumber].m_kitIfPercussion) {
            if (auto instrument = percussionSet->tryGetInstrumentFromPitch(pitch)) {
                addToChannel<bw_music::PercussionOnEvent>(channelNumber, timeSinceLastTrackEvent, *instrument,
                                                          velocity);
                return true;
            }
            return false;
        } else {
            addToChannel<bw_music::NoteOnEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
            return true;
        }
    }

    bool addNoteOff(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent, bw_music::Pitch pitch,
                    bw_music::Velocity velocity) {
        if (const bw_music::PercussionSetWithPitchMap* const percussionSet =
                m_channelSetup[channelNumber].m_kitIfPercussion) {
            if (auto instrument = percussionSet->tryGetInstrumentFromPitch(pitch)) {
                addToChannel<bw_music::PercussionOffEvent>(channelNumber, timeSinceLastTrackEvent, *instrument,
                                                           velocity);
                return true;
            }
            return false;
        } else {
            addToChannel<bw_music::NoteOffEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
            return true;
        }
    }

    /// All channels share the duration of the MIDI track.
    void setDurationsForAllChannels(bw_music::ModelDuration timeToEndOfTrackEvent) {
        const bw_music::ModelDuration duration = m_timeSinceStart + timeToEndOfTrackEvent;
        for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
            if (m_channels[channelNumber] != nullptr) {
                m_channels[channelNumber]->m_trackDuration = duration;
            }
        }
    }

  private:
    struct PerChannelInfo {
        bw_music::TrackBuilder m_track;
        bw_music::ModelDuration m_timeOfLastEvent;
        bw_music::ModelDuration m_trackDuration = 0;
    };

    PerChannelInfo* getChannel(unsigned int channelNumber) {
        assert(0 <= channelNumber);
        assert(channelNumber < MAX_CHANNELS);
        auto& channel = m_channels[channelNumber];

        if (channel == nullptr) {
            channel = std::make_unique<PerChannelInfo>();
        }
        return channel.get();
    }

    template <typename EVENT_TYPE, typename... ARGS>
    void addToChannel(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent, ARGS&&... args) {
        PerChannelInfo* channel = getChannel(channelNumber);

        m_timeSinceStart += timeSinceLastTrackEvent;
        channel->m_track.addEvent(
            EVENT_TYPE{m_timeSinceStart - channel->m_timeOfLastEvent, std::forward<ARGS>(args)...});
        channel->m_timeOfLastEvent = m_timeSinceStart;
    }

  public:
    bw_music::ModelDuration m_timeSinceStart;

    std::array<std::unique_ptr<PerChannelInfo>, MAX_CHANNELS> m_channels;

    const std::array<ChannelSetup, 16>& m_channelSetup;
};

template <typename STREAMLIKE> void smf::SmfParser::logByteSequence(STREAMLIKE log, int length) {
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    for (auto i = 0; i < length - 1; ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(getNext());
    }
}

void smf::SmfParser::readFullMessageIntoBuffer(std::uint32_t length) {
    m_messageBuffer.resize(length);
    for (int i = 0; i < length; ++i) {
        m_messageBuffer[i] = getNext();
    }
}

template <std::size_t N> bool smf::SmfParser::isMessageBufferMessage(const std::array<std::int16_t, N>& message) const {
    if (m_messageBuffer.size() != message.size()) {
        return false;
    }
    for (int i = 0; i < message.size(); ++i) {
        if (message[i] != -1) {
            if (m_messageBuffer[i] != message[i]) {
                return false;
            }
        }
    }
    return true;
}

template <typename STREAMLIKE> void smf::SmfParser::logMessageBuffer(STREAMLIKE log) const {
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[0]);
    for (auto i = 1; i < m_messageBuffer.size(); ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(m_messageBuffer[i]);
    }
}

void smf::SmfParser::readSysExEvent() {
    auto length = readVariableLengthQuantity();
    if (length < 1) {
        logByteSequence(m_userLogger.logWarning() << "Skipping SysEx message with invalid length: ", length);
        return;
    }
    readFullMessageIntoBuffer(length);
    const babelwires::Byte headerId = m_messageBuffer[0];
    if (headerId == 0x7E) {
        // Universal SysEx
        if (length < 4) {
            logByteSequence(m_userLogger.logWarning() << "Skipping Universal SysEx message with invalid length: ",
                            length);
            return;
        }
        // Universal Non-Real Time SysEx
        /*const babelwires::Byte deviceId = m_messageBuffer[1]*/
        const babelwires::Byte subId1 = m_messageBuffer[2];
        const babelwires::Byte subId2 = m_messageBuffer[3];
        if (subId1 == 0x09) {
            // General MIDI message
            if (subId2 == 0x01) {
                // General MIDI On
                setGMSpec(GMSpecType::Value::GM);
                babelwires::logDebug() << "General MIDI On";
            } else if (subId2 == 0x02) {
                // General MIDI Off
                babelwires::logDebug() << "General MIDI Off";
            } else if (subId2 == 0x03) {
                // General MIDI 2 On
                setGMSpec(GMSpecType::Value::GM2);
                babelwires::logDebug() << "General MIDI 2 On";
            } else {
                babelwires::logDebug() << "Ignoring unrecognized General MIDI SysEx message";
            }
            if (m_messageBuffer[4] != 0xF7) {
                m_userLogger.logWarning() << "Improperly terminated General MIDI SysEx message";
            }
            return;
        }
    } else if (headerId == 0x41) {
        // Roland SysEx
        const unsigned int messageSize = m_messageBuffer.size();
        if (m_messageBuffer[messageSize - 1] != 0xF7) {
            m_userLogger.logWarning() << "Improperly terminated Roland SysEx message";
        }
        // Checksum
        babelwires::Byte checkSum = 0;
        for (int i = 4; i < messageSize - 2; ++i) {
            // This can overflow without problems.
            checkSum += m_messageBuffer[i];
        }
        if (m_messageBuffer[messageSize - 2] != ((0x80 - (checkSum % 0x80)) % 0x80)) {
            m_userLogger.logWarning() << "Ignoring Roland SysEx message with invalid checksum";
            return;
        }
        if (isMessageBufferMessage(
                std::array<std::int16_t, 10>{0x41, -1, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7})) {
            setGMSpec(GMSpecType::Value::GS);
            babelwires::logDebug() << "Roland GS Reset";
            return;
        }
        if (isMessageBufferMessage(std::array<std::int16_t, 10>{0x41, -1, 0x42, 0x12, 0x40, -1, 0x15, -1, -1, 0xF7}) &&
            ((m_messageBuffer[5] & 0xf0) == 0x10)) {
            // Use for rhythm part.
            const babelwires::Byte blockNumber = m_messageBuffer[5] & 0x0f;
            const babelwires::Byte value = m_messageBuffer[7];
            if (value > 2) {
                m_userLogger.logWarning()
                    << "Ignoring Roland SysEx use for rhythm part message with out of range value";
            } else {
                setGsPartMode(blockNumber, value);
            }
            return;
        }
    } else if (headerId == 0x43) {
        // Yamaha SysEx
        if (isMessageBufferMessage(std::array<std::int16_t, 8>{0x43, -1, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7})) {
            setGMSpec(GMSpecType::Value::XG);
            babelwires::logDebug() << "Yamaha XG Reset";
            return;
        }
    }
    logMessageBuffer(babelwires::logDebug() << "Ignoring unrecognized SysEx event! ");
}

void smf::SmfParser::readSysExEventContinuation() {
    auto length = readVariableLengthQuantity();
    logByteSequence(babelwires::logDebug() << "Ignoring continued SysEx message", length);
}

void smf::SmfParser::readSequencerSpecificEvent(int length) {
    if (length <= 1) {
        logByteSequence(m_userLogger.logWarning() << "Skipping sequencer specific event with invalid length: ", length);
        return;
    }
    assert(length <= 255 && "Length was expected to be held in one byte");
    std::array<babelwires::Byte, 255> eventBytes;
    for (int i = 0; i < length; ++i) {
        eventBytes[i] = getNext();
    }
    auto log = babelwires::logDebug();
    int byteIndex = 0;
    if (eventBytes[0] == 0x43) {
        if ((length >= 3) && (eventBytes[1] == 0x7B)) {
            // XF Events
            byteIndex = 3;
            switch (eventBytes[2]) {
                case 0x00:
                    log << "Ignored XF Version ID: ";
                    break;
                case 0x01:
                    log << "Ignored XF Chord Event: ";
                    break;
                case 0x02:
                    log << "Ignored XF Rehearsal Mark: ";
                    break;
                case 0x03:
                    log << "Ignored XF Phrase Mark: ";
                    break;
                case 0x04:
                    log << "Ignored XF Max Phrase Mark: ";
                    break;
                case 0x05:
                    log << "Ignored XF Fingered Number: ";
                    break;
                case 0x0C:
                    log << "Ignored XF Guide Track Flag: ";
                    break;
                case 0x10:
                    log << "Ignored XF Information Flag for Guitar: ";
                    break;
                case 0x12:
                    log << "Ignored XF Chord Voicing for Guitar: ";
                    break;
                case 0x7F:
                    log << "Ignored XF Song Data Number: ";
                    break;
                default:
                    log << "Ignored unrecognized XF event: ";
                    byteIndex = 2;
                    break;
            }
        } else if ((length >= 5) && (eventBytes[1] == 0x73)) {
            // Yamaha META event: See CVP900 Data List manual.
            byteIndex = 5;
            if (eventBytes[2] == 0x0A) {
                switch (static_cast<unsigned int>(eventBytes[3] << 8) + eventBytes[4]) {
                    case 0x0004:
                        log << "Ignored Yamaha Meta-event Start Measure Number: ";
                        break;
                    case 0x0005:
                        log << "Ignored Yamaha Meta-event Track Information: ";
                        break;
                    case 0x0006:
                        log << "Ignored Yamaha Meta-event Offset Volume: ";
                        break;
                    case 0x0007:
                        log << "Ignored Yamaha Meta-event Song Offset Measure: ";
                        break;
                    default:
                        log << "Ignored Yamaha Meta-event XF event: ";
                        byteIndex = 2;
                        break;
                }
            } else if (eventBytes[2] == 0x0C) {
                log << "Ignored Yamaha Meta-event Style Name: ";
                byteIndex = 3;
            } else if (eventBytes[2] == 0x0D) {
                log << "Ignored Yamaha Meta-event Song OTS: ";
                byteIndex = 3;
            } else {
                byteIndex = 2;
                log << "Ignored unrecognized Yamaha Meta-event: ";
            }
        } else {
            log << "Ignored Yamaha sequencer specific meta-event: ";
            byteIndex = 2;
        }
    } else {
        log << "Ignored sequencer specific meta-event: ";
    }
    log << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(eventBytes[byteIndex]);
    for (auto i = byteIndex + 1; i < length; ++i) {
        log << ", " << std::setfill('0') << std::setw(2) << static_cast<int>(eventBytes[i]);
    }
}

void smf::SmfParser::readControlChange(unsigned int channelNumber) {
    const babelwires::Byte controllerNumber = getNext();
    const babelwires::Byte value = getNext();
    switch (controllerNumber) {
        case 0x00:
            // bank select MSB
            setBankMSB(channelNumber, value);
            break;
        case 0x20:
            // bank select LSB
            setBankLSB(channelNumber, value);
            break;
        default:
            break;
    }
}

void smf::SmfParser::readProgramChange(unsigned int channelNumber) {
    const babelwires::Byte newProgram = getNext();
    setProgram(channelNumber, newProgram);
}

void smf::SmfParser::readTrack(int trackIndex, TrackSplitter& tracks, bool hasMainMetadata) {
    readByteSequence("MTrk");
    const std::uint32_t trackLength = readU32();
    const int currentIndex = m_dataSource.getAbsolutePosition();

    bw_music::ModelDuration timeSinceLastNoteEvent = 0;
    babelwires::Byte lastStatusByte = 0;
    while ((m_dataSource.getAbsolutePosition() - currentIndex) < trackLength) {
        timeSinceLastNoteEvent += readModelDuration();

        // Peek in case running status should be used.
        babelwires::Byte statusByte = peekNext();
        if ((statusByte & 0x80) || (lastStatusByte == 0)) {
            // A new status byte, so consume the status byte.
            getNext();

            // Buffer stores the status when a Voice Category Status (ie, 0x80 to 0xEF) is received.
            // Buffer is cleared when a System Common Category Status (ie, 0xF0 to 0xF7) is received.
            // Nothing is done to the buffer when a RealTime Category message is received.

            if ((statusByte >= 0x80) && (statusByte <= 0xEF)) {
                // Voice category status
                lastStatusByte = statusByte;
            } else if ((statusByte >= 0xF0) && (statusByte <= 0xF7)) {
                // System common category status
                lastStatusByte = 0;
            }
        } else {
            // Running status.
            statusByte = lastStatusByte;
        }

        const babelwires::Byte statusHi = statusByte >> 4;
        const babelwires::Byte statusLo = statusByte & 0xf;

        // Implementation note:
        // I use the debug log where an event is valid, but I haven't implemented support for it yet.
        // I use the user log where an event is invalid, so it could never be parsed correctly.

        switch (statusHi) {
            case 0b1111: {
                if (statusLo == 0x00) {
                    readSysExEvent();
                } else if (statusLo == 0x07) {
                    readSysExEventContinuation();
                } else if (statusLo == 0x0f) {
                    // Meta-event.
                    const babelwires::Byte type = getNext();
                    const std::uint32_t length = readVariableLengthQuantity();
                    switch (type) {
                        case 0x00: // Sequence number
                        {
                            if (length != 2) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping sequence number meta-event with incorrect length: ",
                                                length);
                            } else {
                                babelwires::logDebug() << "Ignored meta-event! Sequence number: " << readU16();
                            }
                            break;
                        }
                        case 0x01: // Text event
                        {
                            babelwires::logDebug() << "Ignored meta-event! Text event: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x02: // Copyright
                        {
                            std::string text = readTextMetaEvent(length);
                            if (hasMainMetadata) {
                                getMidiMetadata().activateAndGetCopyR().set(text);
                            }
                            break;
                        }
                        case 0x03: // Sequence or track name.
                        {
                            std::string text = readTextMetaEvent(length);
                            if (hasMainMetadata) {
                                getMidiMetadata().activateAndGetName().set(text);
                            }
                            break;
                        }
                        case 0x04: // Instrument name
                        {
                            babelwires::logDebug()
                                << "Ignored MIDI Event! Instrument name: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x05: // Lyric
                        {
                            babelwires::logDebug() << "Ignored meta-event! Lyric: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x06: // Marker
                        {
                            babelwires::logDebug() << "Ignored meta-event! Marker: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x07: // Cue point
                        {
                            babelwires::logDebug() << "Ignored meta-event! Cue point: " << readTextMetaEvent(length);
                            break;
                        }
                        case 0x20: // Channel prefix
                        {
                            if (length != 1) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping channel prefix meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Channel prefix: ",
                                                length);
                            }
                            break;
                        }
                        case 0x2F: // End of track.
                        {
                            // Finished.
                            if ((m_dataSource.getAbsolutePosition() - currentIndex) != trackLength) {
                                throw babelwires::ParseException()
                                    << "MIDI track " << trackIndex << " had an unexpected end-of-track event";
                            }
                            if (length != 0) {
                                // Not a good idea to skip end of track events.
                                logByteSequence(
                                    m_userLogger.logWarning()
                                        << "End of Track meta-event has incorrect length. Will try use it anyway.",
                                    length);
                            }
                            tracks.setDurationsForAllChannels(timeSinceLastNoteEvent);
                            return;
                        }
                        case 0x51: // Set tempo
                        {
                            if (length != 3) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Tempo meta-event with incorrect length",
                                                length);
                            } else {
                                const std::uint32_t tempoValue = readU24();
                                if (hasMainMetadata) {
                                    readTempoEvent(tempoValue);
                                }
                            }
                            break;
                        }
                        case 0x54: // SMPTE offset
                        {
                            if (length != 5) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping SMPTE Offset meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! SMPTE Offset: ", length);
                            }
                            break;
                        }
                        case 0x58: // Time signature
                        {
                            if (length != 4) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Time Signature meta-event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Time signature: ",
                                                length);
                            }
                            break;
                        }
                        case 0x59: // Key signature
                        {
                            if (length != 2) {
                                logByteSequence(m_userLogger.logWarning()
                                                    << "Skipping Key Signature event with incorrect length",
                                                length);
                            } else {
                                logByteSequence(babelwires::logDebug() << "Ignored meta-event! Key signature: ",
                                                length);
                            }
                            break;
                        }
                        case 0x7F: // Sequence specific event
                        {
                            readSequencerSpecificEvent(length);
                            break;
                        }
                        default: // Unknown meta-event type
                        {
                            // This isn't in the spec, so warn: Perhaps BabelWires-Music is out-of-date.
                            logByteSequence(m_userLogger.logWarning() << "Skipping unknown meta-event of type "
                                                                      << std::hex << (int)type << ": ",
                                            length);
                            break;
                        }
                    }
                } else {
                    throw babelwires::ParseException()
                        << "Unrecognized MIDI message with status byte " << static_cast<int>(statusByte);
                }
                break;
            }
            case 0b1000: // Note off.
            {
                const bw_music::Pitch pitch = getNext();
                const bw_music::Velocity velocity = getNext();
                // TODO If a NoteOn was skipped, we would need to skip the corresponding note off.
                if (tracks.addNoteOff(statusLo, timeSinceLastNoteEvent, pitch, velocity)) {
                    timeSinceLastNoteEvent = 0;
                }
                break;
            }
            case 0b1001: // Note on.
            {
                const bw_music::Pitch pitch = getNext();
                const bw_music::Velocity velocity = getNext();
                if (velocity != 0) {
                    if (tracks.addNoteOn(statusLo, timeSinceLastNoteEvent, pitch, velocity)) {
                        timeSinceLastNoteEvent = 0;
                    }
                } else {
                    if (tracks.addNoteOff(statusLo, timeSinceLastNoteEvent, pitch, velocity)) {
                        timeSinceLastNoteEvent = 0;
                    }
                }
                break;
            }
            case 0b1010: // Polyphonic key pressure Aftertouch.
            {
                skipBytes(2);
                break;
            }
            case 0b1011: // Control change.
            {
                readControlChange(statusLo);
                break;
            }
            case 0b1110: // Pitch wheel
            {
                skipBytes(2);
                break;
            }
            case 0b1100: // Program change
            {
                readProgramChange(statusLo);
                break;
            }
            case 0b1101: // Channel pressure
            {
                skipBytes(1);
                break;
            }
            default: {
                throw babelwires::ParseException()
                    << "Unrecognized MIDI message with status byte " << static_cast<int>(statusByte);
            }
        }
    }

    // We've read too far.
    throw babelwires::ParseException() << "Read all of track " << trackIndex
                                       << " without finding an end-of-track event";
}

void smf::SmfParser::readFormat0Sequence() {
    if (m_numTracks != 1) {
        throw babelwires::ParseException()
            << "A format 0 Standard MIDI file claims to have " << m_numTracks << " tracks but it should only have 1";
    }
    TrackSplitter splitTracks(m_channelSetup);
    readTrack(0, splitTracks, true);
    auto tracks = getSmfSequence().getTrcks0();
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        const auto& perChannelInfoPtr = splitTracks.m_channels[channelNumber];
        if (perChannelInfoPtr != nullptr) {
            tracks.activateAndGetTrack(channelNumber).set(perChannelInfoPtr->m_track.finishAndGetTrack(perChannelInfoPtr->m_trackDuration));
        }
    }
}

void smf::SmfParser::readFormat1SequenceTrack(MidiTrackAndChannel::Instance& track,
                                              bool hasMainMetadata) {
    TrackSplitter splitTrack(m_channelSetup);
    readTrack(0, splitTrack, hasMainMetadata);

    // Convert the builders to actual tracks.
    std::array<bw_music::Track, 16> tracks;

    // If this is a format 1 track with multiple channels (rare but possible), privilege the
    // channel with the most events.
    int privilegedTrack = -1;
    int maxNumEvents = 0;
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        if (splitTrack.m_channels[channelNumber] != nullptr) {
            tracks[channelNumber] = splitTrack.m_channels[channelNumber]->m_track.finishAndGetTrack();
            if (tracks[channelNumber].getNumEvents() > maxNumEvents) {
                privilegedTrack = channelNumber;
                maxNumEvents = tracks[channelNumber].getNumEvents();
            }
        }
    }
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        if (channelNumber == privilegedTrack) {
            track.getChan().set(channelNumber);
            track.getTrack().set(std::move(tracks[channelNumber]));
        } else if (splitTrack.m_channels[channelNumber] != nullptr) {
            // All other tracks are added as extra.
            track.activateAndGetTrack(channelNumber).set(std::move(tracks[channelNumber]));
        }
    }
}

void smf::SmfParser::readFormat1Sequence() {
    auto tracks = getSmfSequence().getTrcks1();
    tracks.setSize(m_numTracks);
    for (int i = 0; i < m_numTracks; ++i) {
        auto track = tracks.getEntry(i);
        readFormat1SequenceTrack(track, (i == 0));
    }
}

smf::MidiMetadata::Instance smf::SmfParser::getMidiMetadata() {
    return getSmfSequence().getMeta();
}

void smf::SmfParser::setGMSpec(GMSpecType::Value gmSpec) {
    for (int i = 0; i < 16; ++i) {
        m_channelSetup[i].m_kitIfPercussion = m_standardPercussionSets.getDefaultPercussionSet(gmSpec, i);
    }
    getMidiMetadata().getSpec().set(gmSpec);
}

void smf::SmfParser::setBankMSB(unsigned int channelNumber, const babelwires::Byte msbValue) {
    m_channelSetup[channelNumber].m_channelSetupInfo.m_bankMSB = msbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setBankLSB(unsigned int channelNumber, const babelwires::Byte lsbValue) {
    m_channelSetup[channelNumber].m_channelSetupInfo.m_bankLSB = lsbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setProgram(unsigned int channelNumber, const babelwires::Byte value) {
    m_channelSetup[channelNumber].m_channelSetupInfo.m_program = value;
    onChangeProgram(channelNumber);
}

void smf::SmfParser::setGsPartMode(unsigned int blockNumber, babelwires::Byte value) {
    // For now, assume the midi channels for each part are unchanged.
    // I'm indexing midi channels from 0.
    const unsigned int channelNumber = s_gsBlockToPartMapping[blockNumber] - 1;
    m_channelSetup[channelNumber].m_channelSetupInfo.m_gsPartMode = value;
    onChangeProgram(channelNumber);
}

/// Right now, just trying to determine which percussionSet is in use if any.
void smf::SmfParser::onChangeProgram(unsigned int channelNumber) {
    ChannelSetup& channelSetup = m_channelSetup[channelNumber];
    const GMSpecType::Value gmSpec = getMidiMetadata().getSpec().get();
    channelSetup.m_kitIfPercussion =
        m_standardPercussionSets.getPercussionSetFromChannelSetupInfo(gmSpec, channelSetup.m_channelSetupInfo);
}

std::unique_ptr<babelwires::ValueTreeRoot> smf::parseSmfSequence(babelwires::DataSource& dataSource,
                                                       const babelwires::ProjectContext& projectContext,
                                                       babelwires::UserLogger& userLogger) {
    SmfParser parser(dataSource, projectContext, userLogger);
    parser.parse();
    return parser.getResult();
}
