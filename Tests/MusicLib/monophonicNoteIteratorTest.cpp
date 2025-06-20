#include <gtest/gtest.h>

#include <MusicLib/Utilities/monophonicNoteIterator.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/TestUtils/testTrackEvents.hpp>

namespace {
    bw_music::Track createTestTrack() {
        bw_music::TrackBuilder track;

        bw_music::NoteOnEvent noteOn;
        bw_music::NoteOffEvent noteOff;

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);
        // End of note

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        track.addEvent(testUtils::TestTrackEvent(1));

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);
        // End of note

        track.addEvent(testUtils::TestTrackEvent(5));

        // Expect this note.
        noteOn.setTimeSinceLastEvent(0);
        noteOn.m_pitch = 50;
        track.addEvent(noteOn);

        track.addEvent(testUtils::TestTrackEvent(1));

        noteOn.setTimeSinceLastEvent(1);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        track.addEvent(testUtils::TestTrackEvent(1));
        track.addEvent(testUtils::TestTrackEvent(1));

        noteOff.setTimeSinceLastEvent(0);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);

        track.addEvent(testUtils::TestTrackEvent(1));

        noteOn.setTimeSinceLastEvent(1);
        noteOn.m_pitch = 40;
        track.addEvent(noteOn);

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 50;
        track.addEvent(noteOff);
        // End of note

        track.addEvent(testUtils::TestTrackEvent(1));

        noteOff.setTimeSinceLastEvent(1);
        noteOff.m_pitch = 40;
        track.addEvent(noteOff);

        track.addEvent(testUtils::TestTrackEvent(1));

        return track.finishAndGetTrack();
    }
} // namespace

TEST(MonophonicNoteIterator, OnOffOnly) {
    bw_music::Track track = createTestTrack();

    auto [it, end] = bw_music::iterateOverMonotonicNotes(track, bw_music::MonophonicNoteIterator::OnOffOnly);

    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->m_pitch, 40);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->getTimeSinceLastEvent(), 0);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->m_pitch, 40);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->getTimeSinceLastEvent(), 1);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->m_pitch, 40);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->getTimeSinceLastEvent(), 0);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->m_pitch, 40);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->getTimeSinceLastEvent(), 2);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOnEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->m_pitch, 50);
    EXPECT_EQ(it->as<bw_music::NoteOnEvent>()->getTimeSinceLastEvent(), 5);

    ++it;
    ASSERT_NE(it, end);
    ASSERT_NE(it->as<bw_music::NoteOffEvent>(), nullptr);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->m_pitch, 50);
    EXPECT_EQ(it->as<bw_music::NoteOffEvent>()->getTimeSinceLastEvent(), 7);

    ++it;
    ASSERT_EQ(it, end);
}
