#include <Tests/TestUtils/seqTestUtils.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <gtest/gtest.h>

void testUtils::addSimpleNotes(const std::vector<bw_music::Pitch>& expectedPitches, bw_music::Track& track) {
    for (auto pitch : expectedPitches) {
        track.addEvent(bw_music::NoteOnEvent{ 0, pitch });
        track.addEvent(bw_music::NoteOffEvent{ babelwires::Rational(1, 4), pitch });
    }
}

void testUtils::testSimpleNotes(const std::vector<bw_music::Pitch>& expectedPitches, const bw_music::Track& track) {
    const bw_music::ModelDuration expectedNoteDuration = babelwires::Rational(1, 4);

    // Note: right now, we only parse notes. As we add parsing of other event types, this test will fail.
    // (A temporarily fix would be to use a FilteredTrackIterator.)
    auto noteIterator = track.begin();
    const auto endIterator = track.end();

    for (unsigned int i = 0; i < expectedPitches.size(); ++i) {
        auto pitch = expectedPitches[i];
        ASSERT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->as<const bw_music::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), 0);
        EXPECT_EQ(noteOn->m_pitch, pitch);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;

        ASSERT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->as<bw_music::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), expectedNoteDuration);
        EXPECT_EQ(noteOff->m_pitch, pitch);
        EXPECT_EQ(noteOff->m_velocity, 64);
        ++noteIterator;
    }
    EXPECT_EQ(noteIterator, endIterator);
    EXPECT_EQ(track.getDuration(), expectedNoteDuration * expectedPitches.size());
}

void testUtils::addNotes(const std::vector<NoteInfo>& notes, bw_music::Track& track) {
    for (auto note : notes) {
        track.addEvent(bw_music::NoteOnEvent{ note.m_noteOnTime, note.m_pitch });
        track.addEvent(bw_music::NoteOffEvent{ note.m_noteOffTime, note.m_pitch });
    }
}

void testUtils::testNotes(const std::vector<NoteInfo>& expectedNotes, const bw_music::Track& track) {
    auto noteIterator = track.begin();
    const auto endIterator = track.end();

    bw_music::ModelDuration durationSoFar;

    for (unsigned int i = 0; i < expectedNotes.size(); ++i) {
        auto note = expectedNotes[i];
        EXPECT_NE(noteIterator, endIterator);
        auto noteOn = noteIterator->as<const bw_music::NoteOnEvent>();
        ASSERT_NE(noteOn, nullptr);
        EXPECT_EQ(noteOn->getTimeSinceLastEvent(), note.m_noteOnTime);
        EXPECT_EQ(noteOn->m_pitch, note.m_pitch);
        EXPECT_EQ(noteOn->m_velocity, 127);
        ++noteIterator;

        EXPECT_NE(noteIterator, endIterator);
        auto noteOff = noteIterator->as<bw_music::NoteOffEvent>();
        ASSERT_NE(noteOff, nullptr);
        EXPECT_EQ(noteOff->getTimeSinceLastEvent(), note.m_noteOffTime);
        EXPECT_EQ(noteOff->m_pitch, note.m_pitch);
        durationSoFar += note.m_noteOnTime + note.m_noteOffTime;
        if (durationSoFar != track.getDuration()) {
            // The last events in the track may be artificial noteOff events to close off truncated groups.
            // In this case, we cannot assume the velocity will be correct.
            EXPECT_EQ(noteOff->m_velocity, 64);
        }
        ++noteIterator;
    }
    EXPECT_EQ(noteIterator, endIterator);
}

void testUtils::addChords(const std::vector<ChordInfo>& chords, bw_music::Track& track) {
    for (auto expectedChord : chords) {
        track.addEvent(bw_music::ChordOnEvent(expectedChord.m_chordOnTime, expectedChord.m_chord));
        track.addEvent(bw_music::ChordOffEvent(expectedChord.m_chordOffTime));
    }
}

void testUtils::testChords(const std::vector<ChordInfo>& expectedChords, const bw_music::Track& track) {
    // TODO: Use filteredTrackIterator to allow testing chords when other events present.
    auto chordIterator = track.begin();
    const auto endIterator = track.end();

    for (unsigned int i = 0; i < expectedChords.size(); ++i) {
        auto expectedChord = expectedChords[i];
        EXPECT_NE(chordIterator, endIterator);
        auto chordOn = chordIterator->as<const bw_music::ChordOnEvent>();
        ASSERT_NE(chordOn, nullptr);
        EXPECT_EQ(chordOn->getTimeSinceLastEvent(), expectedChord.m_chordOnTime);
        EXPECT_EQ(chordOn->m_chord, expectedChord.m_chord);
        ++chordIterator;

        EXPECT_NE(chordIterator, endIterator);
        auto chordOff = chordIterator->as<bw_music::ChordOffEvent>();
        ASSERT_NE(chordOff, nullptr);
        EXPECT_EQ(chordOff->getTimeSinceLastEvent(), expectedChord.m_chordOffTime);
        ++chordIterator;
    }
    EXPECT_EQ(chordIterator, endIterator);
}

void testUtils::testNotesAndChords(const std::vector<bw_music::TrackEventHolder>& expectedEvents, const bw_music::Track& track) {
    auto it = track.begin();
    const auto end = track.end();

    for (auto e : expectedEvents) {
        ASSERT_NE(it, end);
        EXPECT_EQ(it->getTimeSinceLastEvent(), e->getTimeSinceLastEvent());
        EXPECT_EQ((it->as<bw_music::NoteOnEvent>() == nullptr), (e->as<bw_music::NoteOnEvent>() == nullptr));
        EXPECT_EQ((it->as<bw_music::NoteOffEvent>() == nullptr), (e->as<bw_music::NoteOffEvent>() == nullptr));
        EXPECT_EQ((it->as<bw_music::ChordOnEvent>() == nullptr), (e->as<bw_music::ChordOnEvent>() == nullptr));
        EXPECT_EQ((it->as<bw_music::ChordOffEvent>() == nullptr), (e->as<bw_music::ChordOffEvent>() == nullptr));
        if (it->as<bw_music::NoteEvent>() != nullptr) {
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_pitch, e->as<bw_music::NoteEvent>()->m_pitch);
            EXPECT_EQ(it->as<bw_music::NoteEvent>()->m_velocity, e->as<bw_music::NoteEvent>()->m_velocity);
        }
        if (it->as<bw_music::ChordOnEvent>() != nullptr) {
            EXPECT_EQ(it->as<bw_music::ChordOnEvent>()->m_chord, e->as<bw_music::ChordOnEvent>()->m_chord);
        }
        ++it;
    }
    EXPECT_EQ(it, end);
}
