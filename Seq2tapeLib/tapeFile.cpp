/**
 * A TapeFile holds a decoded version of the audio storage for a sequence.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Seq2tapeLib/tapeFile.hpp>

#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/exceptions.hpp>
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

    std::uint16_t readU16(babelwires::DataSource& dataSource) {
        std::uint16_t hi = dataSource.getNextByte();
        std::uint16_t lo = dataSource.getNextByte();
        return (hi << 8) | lo;
    }

    void writeU32(std::ostream& stream, std::uint32_t x) {
        stream.put(x >> 24);
        stream.put((x >> 16) & 255);
        stream.put((x >> 8) & 255);
        stream.put(x & 255);
    }

    std::uint32_t readU32(babelwires::DataSource& dataSource) {
        std::uint32_t a = dataSource.getNextByte();
        std::uint32_t b = dataSource.getNextByte();
        std::uint32_t c = dataSource.getNextByte();
        std::uint32_t d = dataSource.getNextByte();
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

    std::string readString(babelwires::DataSource& dataSource) {
        std::uint16_t size = readU16(dataSource);
        std::string result(size, '\0');
        for (int i = 0; i < size; ++i) {
            result[i] = dataSource.getNextByte();
        }
        return result;
    }

    babelwires::LongId readIdentifier(babelwires::DataSource& dataSource) {
        const std::string s = readString(dataSource);
        auto result = babelwires::LongId::deserializeFromString(s);
        THROW_ON_ERROR(result, babelwires::ParseException);
        return *result;
    }

} // namespace

void seq2tape::TapeFile::write(std::ostream& stream) const {
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

seq2tape::TapeFile::TapeFile(babelwires::DataSource& dataSource)
    : m_formatIdentifier("_bad_")
{
    for (int i = 0; i < strlen(s_seq2tapePrefix); ++i) {
        if (dataSource.getNextByte() != s_seq2tapePrefix[i]) {
            throw babelwires::IoException() << "Not a valid seq2tape file";
        }
    }
    const int v = dataSource.getNextByte();
    if ((dataSource.getNextByte() != ' ') || (v <= '0') || (v > s_seq2tapeVersion)) {
        throw babelwires::IoException()
            << "Unrecognized version. This program only handles seq2tape files up to version " << s_seq2tapeVersion;
    }
    m_formatIdentifier = readIdentifier(dataSource);
    m_name = readString(dataSource);
    m_copyright = readString(dataSource);
    std::uint16_t numDataFiles = readU16(dataSource);
    for (int i = 0; i < numDataFiles; ++i) {
        const std::uint32_t size = readU32(dataSource);
        m_dataFiles.emplace_back(std::make_unique<DataFile>());
        for (int j = 0; j < size; ++j) {
            m_dataFiles.back()->emplace_back(dataSource.getNextByte());
        }
    }
}
