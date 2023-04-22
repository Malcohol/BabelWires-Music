/**
 * Representations of pitch in SeqWires.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <SeqWiresLib/pitch.hpp>

#include <Common/Identifiers/identifierRegistry.hpp>
#include <Common/exceptions.hpp>

#include <cassert>
#include <cctype>
#include <charconv>
#include <sstream>
#include <iomanip>

ENUM_DEFINE_ENUM_VALUE_SOURCE(PITCH_CLASS_VALUES);

seqwires::PitchClass::PitchClass()
    : Enum(ENUM_IDENTIFIER_VECTOR(PITCH_CLASS_VALUES), 0) {}

#define JUST_IDENTIFIERS(A, B, C) #A,
#define JUST_NAMES(A, B, C) B,

namespace {
    const char* s_pitchClassIdentifiers[static_cast<unsigned int>(seqwires::PitchClass::Value::NUM_VALUES)] = {
        PITCH_CLASS_VALUES(JUST_IDENTIFIERS)};
    const char* s_pitchClassNames[static_cast<unsigned int>(seqwires::PitchClass::Value::NUM_VALUES)] = {
        PITCH_CLASS_VALUES(JUST_NAMES)};
} // namespace

std::string seqwires::PitchClass::valueToString(PitchClass::Value p) {
    return s_pitchClassNames[static_cast<unsigned int>(p)];
}

std::string seqwires::pitchToString(Pitch p) {
    const int pitchClass = p % 12;
    const int octave = p / 12;

    std::ostringstream stringOut;
    stringOut << PitchClass::valueToString(static_cast<PitchClass::Value>(pitchClass)) << octave;

    return stringOut.str();
}

seqwires::Pitch seqwires::stringToPitch(std::string_view s) {
    const char* it = s.data();
    const char* const end = s.data() + s.length();
    while ((it != end) && std::isspace(*it)) {
        ++it;
    }

    if (it == end) {
        throw babelwires::ParseException() << "No contents when parsing pitch value";
    }

    int p = 0;

    switch (*it) {
        case 'a':
        case 'A':
            p = 9;
            break;
        case 'b':
        case 'B':
            p = 11;
            break;
        case 'c':
        case 'C':
            p = 0;
            break;
        case 'd':
        case 'D':
            p = 2;
            break;
        case 'e':
        case 'E':
            p = 4;
            break;
        case 'f':
        case 'F':
            p = 5;
            break;
        case 'g':
        case 'G':
            p = 7;
            break;
        default:
            throw babelwires::ParseException() << "Unexpected character when parsing pitch value";
    }
    ++it;
    if ((it != end) && (*it == '#')) {
        ++p;
        ++it;
    }
    if ((it == end) || std::isspace(*it)) {
        throw babelwires::ParseException() << "No octave value when parsing pitch value";
    }
    int octave = 0;
    const auto result = std::from_chars(it, end, octave);
    if (result.ec != std::errc()) {
        throw babelwires::ParseException() << "Could not parse octave when parsing pitch value";
    }
    if ((octave < 0) || (octave > 10)) {
        throw babelwires::ParseException() << "Octave out of range when parsing pitch value";
    }
    it = result.ptr;
    while (it != end) {
        if (!std::isspace(*it)) {
            throw babelwires::ParseException() << "Trailing characters when parsing pitch value";
        }
        ++it;
    }
    p += octave * 12;
    assert(p >= 0);
    if (p > 127) {
        // G#10, A10, A#10 and B10 are unrepresentable
        throw babelwires::ParseException() << "Note out of range when parsing pitch value";
    }
    return static_cast<Pitch>(p);
}

seqwires::PitchClass::Value seqwires::pitchToPitchClass(seqwires::Pitch p) {
    return static_cast<seqwires::PitchClass::Value>(p % 12);
}

namespace {
    std::string getSpecialNames(seqwires::Pitch p) {
        switch (p) {
            case 60:
                return " (middle C)";
            case 69:
                return " (A440)";
            default:
                return "";
        }
    }

    std::vector<babelwires::ShortId> getPitchEnumValues() {
        babelwires::IdentifiersSource sourceData;
        sourceData.reserve(127);
        // The last two characters will be the pitch.
        const std::string uuidPrefix = "e99dc874-1b68-4de9-97fa-056006373d";
        for (seqwires::Pitch p = 0; p < 127; ++p) {
            const int pitchClass = p % 12;
            const int octave = p / 12;
            babelwires::ShortId identifierSource((std::ostringstream() << s_pitchClassIdentifiers[pitchClass] << octave).str());
            std::string nameSource = (std::ostringstream() << s_pitchClassNames[pitchClass] << octave << getSpecialNames(p)).str();
            babelwires::Uuid uuid((std::ostringstream() << uuidPrefix << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(p)).str());
            sourceData.emplace_back(std::tuple{identifierSource, nameSource, uuid});
        };
        return REGISTERED_ID_VECTOR(sourceData);
    }
} // namespace

seqwires::PitchEnum::PitchEnum()
    : Enum(getPitchEnumValues(), 60) {}
