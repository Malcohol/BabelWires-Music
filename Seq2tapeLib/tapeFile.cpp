/**
 * A TapeFile holds a decoded version of the audio storage for a sequence.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/tapeFile.hpp>

#include <BaseLib/IO/dataSink.hpp>
#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/Result/resultDSL.hpp>
#include <BaseLib/common.hpp>

#include <cassert>
#include <cstring>
#include <sstream>

seq2tape::TapeFile::TapeFile(babelwires::LongId formatIdentifier)
    : m_formatIdentifier(formatIdentifier) {}

babelwires::LongId seq2tape::TapeFile::getFormatIdentifier() const {
    return m_formatIdentifier;
}

std::string seq2tape::TapeFile::getName() const {
    return m_name;
}

void seq2tape::TapeFile::setName(std::string name) {
    m_name = std::move(name);
}

std::string seq2tape::TapeFile::getCopyright() const {
    return m_copyright;
}

void seq2tape::TapeFile::setCopyright(std::string copyright) {
    m_copyright = std::move(copyright);
}

int seq2tape::TapeFile::getNumDataFiles() const {
    return m_dataFiles.size();
}

const seq2tape::TapeFile::DataFile& seq2tape::TapeFile::getDataFile(int i) const {
    return *m_dataFiles.at(i);
}

void seq2tape::TapeFile::addDataFile(std::unique_ptr<DataFile> dataFile) {
    m_dataFiles.emplace_back(std::move(dataFile));
}

namespace {
    /// Human readable header.
    const char s_seq2tapePrefix[] = "Seq2tape v";
    /// Human readable version.
    const char s_seq2tapeVersion = '1';

    void writeU16(std::ostream& stream, std::uint16_t x) {
        stream.put(x >> 8);
        stream.put(x & 255);
    }

    babelwires::ResultT<std::uint16_t> readU16(babelwires::DataSource& dataSource) {
        ASSIGN_OR_ERROR(const std::uint16_t hi, dataSource.getNextByte());
        ASSIGN_OR_ERROR(const std::uint16_t lo, dataSource.getNextByte());
        return (hi << 8) | lo;
    }

    void writeU32(std::ostream& stream, std::uint32_t x) {
        stream.put(x >> 24);
        stream.put((x >> 16) & 255);
        stream.put((x >> 8) & 255);
        stream.put(x & 255);
    }

    babelwires::ResultT<std::uint32_t> readU32(babelwires::DataSource& dataSource) {
        ASSIGN_OR_ERROR(const std::uint32_t a, dataSource.getNextByte());
        ASSIGN_OR_ERROR(const std::uint32_t b, dataSource.getNextByte());
        ASSIGN_OR_ERROR(const std::uint32_t c, dataSource.getNextByte());
        ASSIGN_OR_ERROR(const std::uint32_t d, dataSource.getNextByte());
        return (a << 24) | (b << 16) | (c << 8) | d;
    }

    void writeString(std::ostream& stream, std::string str) {
        assert((str.size() < (1 << 16) - 1) && "String too long");
        writeU16(stream, str.size());
        stream << str;
    }

    void writeIdentifier(std::ostream& stream, babelwires::LongId identifier) {
        std::ostringstream os;
        os << identifier;
        writeString(stream, os.str());
    }

    babelwires::ResultT<std::string> readString(babelwires::DataSource& dataSource) {
        ASSIGN_OR_ERROR(const std::uint16_t size, readU16(dataSource));
        std::string result(size, '\0');
        for (int i = 0; i < size; ++i) {
            ASSIGN_OR_ERROR(result[i], dataSource.getNextByte());
        }
        return result;
    }

    babelwires::ResultT<babelwires::LongId> readIdentifier(babelwires::DataSource& dataSource) {
        ASSIGN_OR_ERROR(const std::string s, readString(dataSource));
        ASSIGN_OR_ERROR(auto result, babelwires::LongId::deserializeFromString(s));
        return result;
    }

} // namespace

void seq2tape::TapeFile::write(babelwires::DataSink& dataSink) const {
    std::ostream& stream = dataSink.stream();
    assert(m_dataFiles.size() > 0);
    for (int i = 0; i < strlen(s_seq2tapePrefix); ++i) {
        stream.put(s_seq2tapePrefix[i]);
    }
    stream.put(s_seq2tapeVersion);
    stream.put(' ');
    writeIdentifier(stream, m_formatIdentifier);
    writeString(stream, m_name);
    writeString(stream, m_copyright);

    writeU16(stream, m_dataFiles.size());
    for (auto&& file : m_dataFiles) {
        writeU32(stream, file->size());
        const char* const bufferAsChars = reinterpret_cast<const char*>(file->data());
        stream.write(bufferAsChars, file->size());
    }
}

babelwires::ResultT<seq2tape::TapeFile> seq2tape::TapeFile::load(babelwires::DataSource& dataSource) {
    TapeFile out;
    for (int i = 0; i < strlen(s_seq2tapePrefix); ++i) {
        ASSIGN_OR_ERROR(const int byte, dataSource.getNextByte());
        if (byte != s_seq2tapePrefix[i]) {
            return babelwires::Error() << "Not a valid seq2tape file";
        }
    }
    ASSIGN_OR_ERROR(const int v, dataSource.getNextByte());
    ASSIGN_OR_ERROR(const int space, dataSource.getNextByte());
    if ((space != ' ') || (v <= '0') || (v > s_seq2tapeVersion)) {
        return babelwires::Error() << "Unrecognized version. This program only handles seq2tape files up to version "
                                   << s_seq2tapeVersion;
    }
    ASSIGN_OR_ERROR(out.m_formatIdentifier, readIdentifier(dataSource));
    ASSIGN_OR_ERROR(out.m_name, readString(dataSource));
    ASSIGN_OR_ERROR(out.m_copyright, readString(dataSource));
    ASSIGN_OR_ERROR(const std::uint16_t numDataFiles, readU16(dataSource));
    for (int i = 0; i < numDataFiles; ++i) {
        ASSIGN_OR_ERROR(const std::uint32_t size, readU32(dataSource));
        out.m_dataFiles.emplace_back(std::make_unique<DataFile>());
        for (int j = 0; j < size; ++j) {
            ASSIGN_OR_ERROR(const std::uint8_t byte, dataSource.getNextByte());
            out.m_dataFiles.back()->emplace_back(byte);
        }
    }
    return out;
}
