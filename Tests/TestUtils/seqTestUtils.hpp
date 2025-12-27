#pragma once

#include <MusicLib/musicTypes.hpp>
#include <MusicLib/chord.hpp>
#include <MusicLib/Types/Track/TrackEvents/trackEvent.hpp>

#include <vector>

namespace bw_music {
    class TrackBuilder;
}

namespace testUtils {
    /// Add notes of the given pitches to the track. Each has quaternote duration.
    void addSimpleNotes(const std::vector<bw_music::Pitch>& expectedPitches, bw_music::TrackBuilder& track);

    /// Get a track containing notes of the given pitches. Each has quaternote duration.
    bw_music::Track getTrackOfSimpleNotes(const std::vector<bw_music::Pitch>& expectedPitches);

    /// Check that there is a sequence of the given pitches in the track, each with quaternote duration.
    void testSimpleNotes(const std::vector<bw_music::Pitch>& expectedPitches, const bw_music::Track& track);

    struct NoteInfo {
        bw_music::Pitch m_pitch;
        bw_music::ModelDuration m_noteDuration = babelwires::Rational(1, 4);
        bw_music::ModelDuration m_gapBeforeNote = 0;
    };

    /// Add notes as described to the track.
    void addNotes(const std::vector<NoteInfo>& notes, bw_music::TrackBuilder& track);

    /// Check that there is a sequence of the given notes in the track.
    void testNotes(const std::vector<NoteInfo>& expectedNotes, const bw_music::Track& track);

    struct ChordInfo {
        bw_music::Chord m_chord;
        bw_music::ModelDuration m_chordDuration = babelwires::Rational(1, 2);
        bw_music::ModelDuration m_gapBeforeChord = 0;
    };

    /// Add the given chords. Each has halfnote duration.
    void addChords(const std::vector<ChordInfo>& chords, bw_music::TrackBuilder& track);

    bw_music::Track getTrackOfChords(const std::vector<ChordInfo>& chords);

    void testChords(const std::vector<ChordInfo>& expectedChords, const bw_music::Track& track);

    void testNotesAndChords(const std::vector<bw_music::TrackEventHolder>& expectedEvents, const bw_music::Track& track);
}
