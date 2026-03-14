/**
 * Representations of pitch in BabelWires-Music.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/pitch.hpp>

#include <BaseLib/Identifiers/identifierRegistry.hpp>

#include <cassert>
#include <cctype>
#include <charconv>
#include <sstream>
#include <iomanip>

ENUM_DEFINE_ENUM_VALUE_SOURCE(bw_music::PitchClass, PITCH_CLASS_VALUES);

bw_music::PitchClass::PitchClass()
    : EnumType(getThisIdentifier(), getStaticValueSet(), 0) {}

#define JUST_IDENTIFIERS(A, B, C) #A,
#define JUST_NAMES(A, B, C) B,

namespace {
    const char* s_pitchClassIdentifiers[static_cast<unsigned int>(bw_music::PitchClass::Value::NUM_VALUES)] = {
        PITCH_CLASS_VALUES(JUST_IDENTIFIERS)};
    const char* s_pitchClassNames[static_cast<unsigned int>(bw_music::PitchClass::Value::NUM_VALUES)] = {
        PITCH_CLASS_VALUES(JUST_NAMES)};
} // namespace

std::string bw_music::PitchClass::valueToString(PitchClass::Value p) {
    return s_pitchClassNames[static_cast<unsigned int>(p)];
}

std::string bw_music::pitchToString(Pitch p) {
    const int pitchClass = p % 12;
    const int octave = p / 12;

    std::ostringstream stringOut;
    stringOut << PitchClass::valueToString(static_cast<PitchClass::Value>(pitchClass)) << octave;

    return stringOut.str();
}

babelwires::ResultT<bw_music::Pitch> bw_music::stringToPitch(std::string_view s) {
    const char* it = s.data();
    const char* const end = s.data() + s.length();
    while ((it != end) && std::isspace(*it)) {
        ++it;
    }

    if (it == end) {
        return babelwires::Error() << "No contents when parsing pitch value";
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
            return babelwires::Error() << "Unexpected character when parsing pitch value";
    }
    ++it;
    if ((it != end) && (*it == '#')) {
        ++p;
        ++it;
    }
    if ((it == end) || std::isspace(*it)) {
        return babelwires::Error() << "No octave value when parsing pitch value";
    }
    int octave = 0;
    const auto result = std::from_chars(it, end, octave);
    if (result.ec != std::errc()) {
        return babelwires::Error() << "Could not parse octave when parsing pitch value";
    }
    if ((octave < 0) || (octave > 10)) {
        return babelwires::Error() << "Octave out of range when parsing pitch value";
    }
    it = result.ptr;
    while (it != end) {
        if (!std::isspace(*it)) {
            return babelwires::Error() << "Trailing characters when parsing pitch value";
        }
        ++it;
    }
    p += octave * 12;
    assert(p >= 0);
    if (p > 127) {
        // G#10, A10, A#10 and B10 are unrepresentable
        return babelwires::Error() << "Note out of range when parsing pitch value";
    }
    return static_cast<Pitch>(p);
}

bw_music::PitchClass::Value bw_music::pitchToPitchClass(bw_music::Pitch p) {
    return static_cast<bw_music::PitchClass::Value>(p % 12);
}

namespace {
    std::string getSpecialNames(bw_music::Pitch p) {
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
        for (bw_music::Pitch p = 0; p < 127; ++p) {
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

bw_music::PitchEnum::PitchEnum()
    : EnumType(getThisIdentifier(), getPitchEnumValues(), 60) {}
