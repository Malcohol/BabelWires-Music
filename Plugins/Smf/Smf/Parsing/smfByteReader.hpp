/**
 * Low-level big-endian byte reading for SMF files from a DataSource
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Smf/Parsing/smfTrackEventConsumer.hpp>

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

} // namespace smf
