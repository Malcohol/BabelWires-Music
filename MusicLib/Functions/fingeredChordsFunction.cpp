/**
 * Function which creates tracks of chord events from tracks of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/fingeredChordsFunction.hpp>

#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <algorithm>
#include <array>

ENUM_DEFINE_ENUM_VALUE_SOURCE(bw_music::FingeredChordsSustainPolicyEnum, FINGERED_CHORDS_SUSTAIN_POLICY);

bw_music::FingeredChordsSustainPolicyEnum::FingeredChordsSustainPolicyEnum()
    : babelwires::EnumType(getThisIdentifier(), getStaticValueSet(), 0) {}

namespace {
    using IntervalSet = std::uint16_t;

    struct IntervalSetToChordType {
        IntervalSet m_intervals;
        int m_chordType;
        
        IntervalSetToChordType(IntervalSet intervals, bw_music::ChordType::Value chordType) 
            : m_intervals(intervals)
            , m_chordType(static_cast<int>(chordType))
        {
        }

        IntervalSetToChordType(IntervalSet intervals, int extraChordValue) 
            : m_intervals(intervals)
            , m_chordType(extraChordValue)
        {
        }

        bool operator<(IntervalSet otherIntervals) const { return m_intervals < otherIntervals; }

        bool operator<(const IntervalSetToChordType& other) const { return m_intervals < other.m_intervals; }
    };

    static constexpr int s_cancelChord = -1;

    /// Represent a chord type by intervals from the root note, which is in the unit position.
    // The "fingered chords" of many arranger-style keyboards are supported, although the root note is always required.
    // The fingering 0b0000100001010001 is ambiguous between M7b5 and M7s11. The latter has a few alternatives, so the former
    // is used.
    const auto recognizedIntervals = std::to_array<IntervalSetToChordType>({
        // clang-format off
        // This must be sorted (the alphabetic sort of a typical editor will work to keep this sorted).
        //    CBAAGGFFEDDCC
        //      # # #  # #
        {0b0000000000000111, s_cancelChord},
        {0b0000000000001101, bw_music::ChordType::Value::m9},
        {0b0000000000010101, bw_music::ChordType::Value::M9},
        {0b0000000001001001, bw_music::ChordType::Value::dim},
        {0b0000000001010001, bw_music::ChordType::Value::b5},
        {0b0000000010000001, bw_music::ChordType::Value::_1p5},
        {0b0000000010000101, bw_music::ChordType::Value::_1p2p5},
        {0b0000000010001001, bw_music::ChordType::Value::m},
        {0b0000000010001101, bw_music::ChordType::Value::m9},
        {0b0000000010010001, bw_music::ChordType::Value::M},
        {0b0000000010010101, bw_music::ChordType::Value::M9},
        {0b0000000010100001, bw_music::ChordType::Value::sus4},
        {0b0000000010101001, bw_music::ChordType::Value::m7_11},
        {0b0000000010101101, bw_music::ChordType::Value::m7_11},
        {0b0000000100010001, bw_music::ChordType::Value::aug},
        {0b0000001000001101, bw_music::ChordType::Value::m6_9},
        {0b0000001000010101, bw_music::ChordType::Value::M6_9},
        {0b0000001001001001, bw_music::ChordType::Value::dim7},
        {0b0000001010000001, bw_music::ChordType::Value::M6},
        {0b0000001010001001, bw_music::ChordType::Value::m6},
        {0b0000001010001101, bw_music::ChordType::Value::m6_9},
        {0b0000001010010001, bw_music::ChordType::Value::M6},
        {0b0000001010010101, bw_music::ChordType::Value::M6_9},
        {0b0000010000001001, bw_music::ChordType::Value::m7},
        {0b0000010000001101, bw_music::ChordType::Value::m7_9},
        {0b0000010000010001, bw_music::ChordType::Value::_7},
        {0b0000010000010011, bw_music::ChordType::Value::_7b9},
        {0b0000010000010101, bw_music::ChordType::Value::_79},
        {0b0000010000011001, bw_music::ChordType::Value::_7s9},
        {0b0000010000100001, bw_music::ChordType::Value::_7sus4},
        {0b0000010001001001, bw_music::ChordType::Value::m7b5},
        {0b0000010001010001, bw_music::ChordType::Value::_7b5},
        {0b0000010001010101, bw_music::ChordType::Value::_7s11},
        {0b0000010010001001, bw_music::ChordType::Value::m7},
        {0b0000010010001101, bw_music::ChordType::Value::m7_9},
        {0b0000010010010001, bw_music::ChordType::Value::_7},
        {0b0000010010010011, bw_music::ChordType::Value::_7b9},
        {0b0000010010010101, bw_music::ChordType::Value::_79},
        {0b0000010010011001, bw_music::ChordType::Value::_7s9},
        {0b0000010010100001, bw_music::ChordType::Value::_7sus4},
        {0b0000010010101001, bw_music::ChordType::Value::m7_11},
        {0b0000010010101101, bw_music::ChordType::Value::m7_11},
        {0b0000010011010001, bw_music::ChordType::Value::_7s11},
        {0b0000010011010101, bw_music::ChordType::Value::_7s11},
        {0b0000010100010001, bw_music::ChordType::Value::_7aug},
        {0b0000010110010001, bw_music::ChordType::Value::_7b13},
        {0b0000011000010001, bw_music::ChordType::Value::_7_13},
        {0b0000011010010001, bw_music::ChordType::Value::_7_13},
        {0b0000100000001001, bw_music::ChordType::Value::mM7},
        {0b0000100000001101, bw_music::ChordType::Value::mM7_9},
        {0b0000100000010001, bw_music::ChordType::Value::M7},
        {0b0000100000010101, bw_music::ChordType::Value::M7_9},
        {0b0000100001001001, bw_music::ChordType::Value::mM7b5},
        {0b0000100001010001, bw_music::ChordType::Value::M7b5},
        {0b0000100001010101, bw_music::ChordType::Value::M7s11},
        {0b0000100010001001, bw_music::ChordType::Value::mM7},
        {0b0000100010001101, bw_music::ChordType::Value::mM7_9},
        {0b0000100010010001, bw_music::ChordType::Value::M7},
        {0b0000100010010101, bw_music::ChordType::Value::M7_9},
        {0b0000100011010001, bw_music::ChordType::Value::M7s11},
        {0b0000100011010001, bw_music::ChordType::Value::M7s11},
        {0b0000100011010101, bw_music::ChordType::Value::M7s11},
        {0b0000100100000001, bw_music::ChordType::Value::Mj7aug},
        {0b0000100100010001, bw_music::ChordType::Value::Mj7aug},
        {0b0001000000000001, bw_music::ChordType::Value::_1p8},
        // clang-format on
    });

    /// Try to identify a chord type which matches the interval.
    /// Returns a ChordType::Value or -1 for cancel chord.
    int getMatchingChordTypeFromIntervals(IntervalSet intervals) {
        // Sortedness is asserted at the beginning of fingeredChordsFunction.
        // For such a small array size, I guessed that binary search would be a good approach,
        // but I didn't do any timings.
        const auto it = std::lower_bound(recognizedIntervals.begin(), recognizedIntervals.end(), intervals);
        if ((it != recognizedIntervals.end()) && (it->m_intervals == intervals)) {
            return it->m_chordType;
        }
        return static_cast<int>(bw_music::ChordType::Value::NotAValue);
    }

    /// The pitches of the set of currently playing notes.
    struct ActivePitches {
        void addPitch(bw_music::Pitch pitch) {
            const auto it = std::upper_bound(m_pitches.begin(), m_pitches.end(), pitch);
            assert(((it == m_pitches.end()) || (*it > pitch)) &&
                   "NoteOnEvent for same pitch as currently playing note");
            m_pitches.insert(it, pitch);
        }

        void removePitch(bw_music::Pitch pitch) {
            const auto it = std::find(m_pitches.begin(), m_pitches.end(), pitch);
            assert((it != m_pitches.end()) && "NoteOffEvent without matching NoteOnEvent");
            m_pitches.erase(it);
        }

        enum class ChordMatch { noChord, matchedChord, cancelChord };

        /// Check whether the currently active pitches match an known IntervalSet or inversion of that IntervalSet.
        ChordMatch getBestMatchChord(bw_music::Chord& bestChordOut) const {
            const unsigned int numPitches = m_pitches.size();
            // TODO Assert min and max match the recognized chords.
            constexpr unsigned int minNumPitches = 2;
            constexpr unsigned int maxNumPitches = 6;
            if ((numPitches < minNumPitches) || (numPitches > maxNumPitches)) {
                return ChordMatch::noChord;
            }
            // The intervals between neighbouring pitches as integers.
            unsigned int pitchDiffs[maxNumPitches - 1] = {0};

            IntervalSet interval = 1;
            {
                unsigned int shift = 0;
                for (unsigned int i = 1; i < numPitches; ++i) {
                    // Bring neighbouring intervals within the octave.
                    pitchDiffs[i - 1] = (m_pitches[i] - m_pitches[i - 1]) % 12;
                    shift += pitchDiffs[i - 1];
                    interval |= IntervalSet(1) << shift;
                }
            }

            // Try each inversion.
            for (unsigned int i = 0; i < numPitches; ++i) {
                const int chordTypeOrCancel = getMatchingChordTypeFromIntervals(interval);
                if (chordTypeOrCancel == s_cancelChord) {
                    return ChordMatch::cancelChord;
                }
                const bw_music::ChordType::Value chordType = static_cast<bw_music::ChordType::Value>(chordTypeOrCancel);
                if (chordType != bw_music::ChordType::Value::NotAValue) {
                    bestChordOut = bw_music::Chord{bw_music::pitchToPitchClass(m_pitches[i]), chordType};
                    return ChordMatch::matchedChord;
                }
                if (i < numPitches - 1) {
                    // Add the old root raised by an octave.
                    interval = (interval + (1 << 12)) >> pitchDiffs[i];
                }
            }
            return ChordMatch::noChord;
        }

        /// The pitches of the current active set, in lowest-to-highest order.
        std::vector<bw_music::Pitch> m_pitches;
    };
} // namespace

bw_music::Track bw_music::fingeredChordsFunction(const Track& sourceTrack, FingeredChordsSustainPolicyEnum::Value sustainPolicy) {
    // Required for getMatchingChordType::ValueFromIntervals
    assert(std::is_sorted(recognizedIntervals.begin(), recognizedIntervals.end()));

    TrackBuilder trackOut;

    ActivePitches activePitches;
    ModelDuration timeSinceLastChordEvent = 0;
    Chord currentChord;

    for (const auto& event : iterateOver<NoteEvent>(sourceTrack)) {
        if (event.getTimeSinceLastEvent() > 0) {
            Chord chordFound;
            const ActivePitches::ChordMatch chordMatch = activePitches.getBestMatchChord(chordFound);
            if (currentChord.m_chordType != ChordType::Value::NotAValue) {
                if (((chordMatch == ActivePitches::ChordMatch::matchedChord) && (currentChord != chordFound))
                || ((chordMatch == ActivePitches::ChordMatch::cancelChord) && (sustainPolicy == FingeredChordsSustainPolicyEnum::Value::Hold))
                || ((chordMatch == ActivePitches::ChordMatch::noChord) && (sustainPolicy == FingeredChordsSustainPolicyEnum::Value::Notes))) {
                    trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
                    currentChord.m_chordType = ChordType::Value::NotAValue;
                    timeSinceLastChordEvent = 0;
                }
            }
            if ((chordMatch == ActivePitches::ChordMatch::matchedChord) && (currentChord != chordFound)) 
            {
                trackOut.addEvent(ChordOnEvent(timeSinceLastChordEvent, chordFound));
                currentChord = chordFound;
                timeSinceLastChordEvent = 0;
            }
        }

        timeSinceLastChordEvent += event.getTimeSinceLastEvent();

        if (const auto* noteOnEvent = event.as<NoteOnEvent>()) {
            activePitches.addPitch(noteOnEvent->m_pitch);
        } else if (const auto* noteOffEvent = event.as<NoteOffEvent>()) {
            activePitches.removePitch(noteOffEvent->m_pitch);
        }
    }
    if (currentChord.m_chordType != ChordType::Value::NotAValue) {
        trackOut.addEvent(ChordOffEvent(timeSinceLastChordEvent));
    }
    return trackOut.finishAndGetTrack(sourceTrack.getDuration());
}
