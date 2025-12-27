/**
 * Function which creates tracks of chord events from tracks of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/fitToChordFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/chord.hpp>

#include <map>

namespace {
    using DegreeAdjustment = std::map<unsigned int, int>;
    using ChordDegreeAdjustments = std::map<bw_music::ChordType::Value, DegreeAdjustment>;

    const ChordDegreeAdjustments chordDegreeAdjustments = {
        // How each chord requires and/or modifies certain degrees of the scale.
        // Degree 1 is assumed and degree 5 doesn't need to be specified if default.
        // The adjustments are in semitones.
        {bw_music::ChordType::Value::M, {{3, 0}}},
        {bw_music::ChordType::Value::M6, {{3, 0}, {6, 0}}},
        {bw_music::ChordType::Value::M7, {{3, 0}, {7, 0}}},
        {bw_music::ChordType::Value::M7s11, {{3, 0}, {7, 0}, {11, 1}}},
        {bw_music::ChordType::Value::M9, {{3, 0}, {9, 0}}},
        {bw_music::ChordType::Value::M7_9, {{3, 0}, {7, 0}, {9, 0}}},
        {bw_music::ChordType::Value::M6_9, {{3, 0}, {6, 0}, {9, 0}}},
        {bw_music::ChordType::Value::aug, {{3, 0}, {5, 1}}},
        {bw_music::ChordType::Value::m, {{3, -1}}},
        {bw_music::ChordType::Value::m6, {{3, -1}, {6, 0}}},
        {bw_music::ChordType::Value::m7, {{3, -1}, {7, -1}}},
        {bw_music::ChordType::Value::m7b5, {{3, -1}, {5, -1}, {7, -1}}},
        {bw_music::ChordType::Value::m9, {{3, -1}, {9, 0}}},
        {bw_music::ChordType::Value::m7_9, {{3, -1}, {7, -1}, {9, 0}}},
        {bw_music::ChordType::Value::m7_11, {{3, -1}, {7, -1}, {11, 0}}},
        {bw_music::ChordType::Value::mM7, {{3, -1}, {7, 0}}},
        {bw_music::ChordType::Value::mM7_9, {{3, -1}, {7, 0}, {9, 0}}},
        {bw_music::ChordType::Value::dim, {{3, -1}, {5, -1}}},
        {bw_music::ChordType::Value::dim7, {{3, -1}, {7, -2}}},
        {bw_music::ChordType::Value::_7, {{3, 0}, {7, -1}}},
        {bw_music::ChordType::Value::_7sus4, {{3, 1}, {7, -1}}},
        {bw_music::ChordType::Value::_7b5, {{3, 0}, {5, -1}, {7, -1}}},
        {bw_music::ChordType::Value::_79, {{3, 0}, {7, -1}, {9, 0}}},
        {bw_music::ChordType::Value::_7s11, {{3, 0}, {7, -1}, {11, 1}}},
        {bw_music::ChordType::Value::_7_13, {{3, 0}, {7, -1}, {13, 0}}},
        {bw_music::ChordType::Value::_7b9, {{3, 0}, {7, -1}, {9, -1}}},
        {bw_music::ChordType::Value::_7b13, {{3, 0}, {7, -1}, {13, -1}}},
        {bw_music::ChordType::Value::_7s9, {{3, 0}, {7, -1}, {9, 1}}},
        {bw_music::ChordType::Value::Mj7aug, {}},
        {bw_music::ChordType::Value::_7aug, {{3, 0}, {5, 1}, {7, -1}}},
        {bw_music::ChordType::Value::_1p8, {}}, // TODO Unsure how to handle this. For now, it is a no-op.
        {bw_music::ChordType::Value::_1p5, {}}, // TODO Unsure how to handle this. For now, it is a no-op.
        {bw_music::ChordType::Value::sus4, {{3, 1}}},
        {bw_music::ChordType::Value::_1p2p5, {{3, -2}}}, // Assuming this should be interpreted as a sus2 chord.
        {bw_music::ChordType::Value::b5, {{5, -1}}},
        {bw_music::ChordType::Value::mM7b5, {{3, -1}, {5, -1}, {7, 0}}},
        {bw_music::ChordType::Value::m6_9, {{3, -1}, {6, 0}, {9, 0}}}};

    // We don't make any assumption about what pitch range the input is in,
    // so we don't know how to distinguish, e.g. degree 2 from degree 9.
    // Thus we flatten all notes to a single octave. This is probably not musically ideal, but it's a reasonable
    // starting point.
    //
    // Note    C  C# D  D# E  F  F# G  G# A  A# B
    // Degree  1     2     3  4     5     6     7
    // Degree  8     9    10 11    12    13    14
    // Index   0  1  2  3  4  5  6  7  8  9 10 11
    const std::array<unsigned int, 15> degreeToPitchIndex = {999, 0, 2, 4, 5, 7, 9, 11, 0, 2, 4, 5, 7, 9, 11};

    using PitchIndexMap = std::array<unsigned int, 12>;

    PitchIndexMap getPitchIndexMap(bw_music::ChordType::Value chordType) {
        using GravityMap = std::array<int, 12>;
        GravityMap gravityMap{};

        PitchIndexMap pitchIndexMap = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

        // Apply adjustments from the modifiers
        const auto it = chordDegreeAdjustments.find(chordType);
        assert(it != chordDegreeAdjustments.end() && "Chord type not found in adjustments map");
        DegreeAdjustment adjustments = it->second;

        // Apply defaults (could provide a policy to make this behaviour configurable).
        adjustments[1] = 0;                 // Degree 1 (root) is always present.
        // Degree 5 is assumed unless specified.
        if (adjustments.find(5) == adjustments.end()) {
            adjustments[5] = 0;
        }

        for (const auto& [degree, adjustment] : adjustments) {
            assert(degree < degreeToPitchIndex.size() && "Degree out of range");
            const unsigned int sourcePitchIndex = degreeToPitchIndex[degree];
            assert(sourcePitchIndex < pitchIndexMap.size() && "Pitch class out of range");
            const unsigned int targetPitchIndex = sourcePitchIndex + adjustment;
            assert(targetPitchIndex < pitchIndexMap.size() && "Target pitch class out of range");
            pitchIndexMap[sourcePitchIndex] = targetPitchIndex;
            gravityMap[targetPitchIndex] = 1;
            gravityMap[sourcePitchIndex] = -1;
        }
        // For remaining chromatic notes, if either neighbouring note is a target of a modifier, pull it towards the
        // target.
        const std::array<unsigned int, 5> chromaticPitchIndices = {1, 3, 6, 8, 10};
        for (unsigned int index : chromaticPitchIndices) {
            if (gravityMap[index] == 0) {
                // If this note is not a target, check its neighbours.
                const unsigned int leftNeighbour = (index + 11) % 12;
                const unsigned int rightNeighbour = (index + 1) % 12;
                if (gravityMap[leftNeighbour] > 0 || gravityMap[rightNeighbour] > 0) {
                    // If either neighbour is a target, pull this note towards the nearest target.
                    // This is left-biased, but I don't think it matters much.
                    pitchIndexMap[index] =
                        (gravityMap[leftNeighbour] > gravityMap[rightNeighbour]) ? leftNeighbour : rightNeighbour;
                }
            }
        }

        return pitchIndexMap;
    }

    struct PitchMap {
        /// This determines which chords get transposed upwards and which chords get transposed down.
        /// Value 6 means that C#..F get transposed upwards, while F#..B get transposed downwards.
        constexpr static int s_topChordRoot = 6;

        PitchMap(const bw_music::Chord& chord)
            : m_pitchIndexMap(getPitchIndexMap(chord.m_chordType)) {
            // Calculate the pitch offset based on the root of the chord.
            const int rootPitchIndex = static_cast<unsigned int>(chord.m_root);
            m_rootOffset = (rootPitchIndex > s_topChordRoot) ? (rootPitchIndex - 12) : rootPitchIndex;
        }

        bw_music::Pitch operator()(bw_music::Pitch pitch) const {
            // Map the pitch to the corresponding index in the chord.
            const auto [octave, pitchIndex] = std::div(static_cast<int>(pitch), 12);
            const int mappedIndex = m_pitchIndexMap[pitchIndex];
            assert(mappedIndex < 12 && "Mapped index out of range");
            int targetPitch = (octave * 12) + mappedIndex + m_rootOffset;
            if (targetPitch < 0) {
                targetPitch += 12;
            } else if (targetPitch >= 127) {
                targetPitch -= 12;
            }

            return static_cast<bw_music::Pitch>(targetPitch);
        }

        PitchIndexMap m_pitchIndexMap;
        int m_rootOffset;
    };

    bw_music::Track fitToChordFunctionInternal(const PitchMap& pitchMap, const bw_music::Track& sourceTrack,
                                               const bw_music::Chord& chord) {
        bw_music::TrackBuilder resultTrack;

        // Iterate through the source track and adjust each note to fit the chord.
        for (const auto& event : sourceTrack) {
            if (const auto note = event.as<bw_music::NoteEvent>()) {
                bw_music::TrackEventHolder newNote = *note;
                newNote->is<bw_music::NoteEvent>().setPitch(pitchMap(note->getPitch()));
                resultTrack.addEvent(newNote.release());
            } else {
                // If the event is not a NoteEvent, just copy it to the result track.
                resultTrack.addEvent(event);
            }
        }

        return resultTrack.finishAndGetTrack(sourceTrack.getDuration());
    }

} // namespace

bw_music::Track bw_music::fitToChordFunction(const Track& sourceTrack, const Chord& chord) {
    const PitchMap pitchMap(chord);

    return fitToChordFunctionInternal(pitchMap, sourceTrack, chord);
}
