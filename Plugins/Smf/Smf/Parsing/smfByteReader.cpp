/**
 * A steppable byte-level parser for Standard MIDI Files, which fires callbacks on a SequenceEventConsumer.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/Parsing/smfByteReader.hpp>

#include <Smf/Parsing/smfEventConsumer.hpp>

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
