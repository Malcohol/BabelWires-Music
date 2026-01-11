#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/fitToChordFunction.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/TestUtils/testTrackEvents.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

class FitToChordFunctionTest : public ::testing::Test {
  protected:
    void SetUp() override {
        m_cMajorChordTrack = createSimpleTrack(60, 64, 67); // C, E, G
        m_lowerLimitTrack = createSimpleTrack(0, 4, 7); // C, E, G at bottom of MIDI range
        m_upperLimitTrack = createSimpleTrack(120, 124, 127); // C, E, G at top of MIDI range
    }

    bw_music::Track createSimpleTrack(bw_music::Pitch pitch0, bw_music::Pitch pitch1, bw_music::Pitch pitch2) {
        bw_music::TrackBuilder trackBuilder;

        trackBuilder.addEvent(bw_music::NoteOnEvent{0, pitch0});
        trackBuilder.addEvent(bw_music::NoteOnEvent{0, pitch1});
        trackBuilder.addEvent(bw_music::NoteOnEvent{0, pitch2});
        trackBuilder.addEvent(testUtils::TestTrackEvent{babelwires::Rational(2), 2}); // Some non-note event.
        trackBuilder.addEvent(bw_music::NoteOffEvent{babelwires::Rational(2), pitch0});
        trackBuilder.addEvent(bw_music::NoteOffEvent{0, pitch1});
        trackBuilder.addEvent(bw_music::NoteOffEvent{0, pitch2});

        return trackBuilder.finishAndGetTrack();
    }

    void testSimpleTrack(const bw_music::Track& track, bw_music::Pitch pitch0, bw_music::Pitch pitch1, bw_music::Pitch pitch2) {
        auto it = track.begin();
        auto noteOn0 = it->tryAs<const bw_music::NoteOnEvent>();
        ASSERT_NE(noteOn0, nullptr);
        EXPECT_EQ(noteOn0->m_pitch, pitch0);
        EXPECT_EQ(noteOn0->getTimeSinceLastEvent(), 0);
        ++it;
        auto noteOn1 = it->tryAs<const bw_music::NoteOnEvent>();
        ASSERT_NE(noteOn1, nullptr);
        EXPECT_EQ(noteOn1->m_pitch, pitch1);
        EXPECT_EQ(noteOn1->getTimeSinceLastEvent(), 0);
        ++it;
        auto noteOn2 = it->tryAs<const bw_music::NoteOnEvent>();
        ASSERT_NE(noteOn2, nullptr);
        EXPECT_EQ(noteOn2->m_pitch, pitch2);
        EXPECT_EQ(noteOn2->getTimeSinceLastEvent(), 0);
        ++it;
        auto nonNoteEvent = it->tryAs<const testUtils::TestTrackEvent>();
        ASSERT_NE(nonNoteEvent, nullptr);
        EXPECT_EQ(nonNoteEvent->m_value, 2);
        EXPECT_EQ(nonNoteEvent->getTimeSinceLastEvent(), babelwires::Rational(2));
        ++it;
        auto noteOff0 = it->tryAs<const bw_music::NoteOffEvent>();
        ASSERT_NE(noteOff0, nullptr);
        EXPECT_EQ(noteOff0->m_pitch, pitch0);
        EXPECT_EQ(noteOff0->getTimeSinceLastEvent(), 2);
        ++it;
        auto noteOff1 = it->tryAs<const bw_music::NoteOffEvent>();
        ASSERT_NE(noteOff1, nullptr);
        EXPECT_EQ(noteOff1->m_pitch, pitch1);
        EXPECT_EQ(noteOff1->getTimeSinceLastEvent(), 0);
        ++it;
        auto noteOff2 = it->tryAs<const bw_music::NoteOffEvent>();
        ASSERT_NE(noteOff2, nullptr);
        EXPECT_EQ(noteOff2->m_pitch, pitch2);
        EXPECT_EQ(noteOff2->getTimeSinceLastEvent(), 0);
        ++it;
        EXPECT_EQ(it, track.end());
    };

    bw_music::Track m_cMajorChordTrack;
    bw_music::Track m_lowerLimitTrack;
    bw_music::Track m_upperLimitTrack;
};

TEST_F(FitToChordFunctionTest, CMajorNoChange) {
    auto result = bw_music::fitToChordFunction(m_cMajorChordTrack, bw_music::Chord{bw_music::PitchClass::Value::C, bw_music::ChordType::Value::M});
    testSimpleTrack(result, 60, 64, 67);
}

TEST_F(FitToChordFunctionTest, AMinor) {
    auto result = bw_music::fitToChordFunction(m_cMajorChordTrack, bw_music::Chord{bw_music::PitchClass::Value::A, bw_music::ChordType::Value::m});
    testSimpleTrack(result, 57, 60, 64); // A, C, E
}

TEST_F(FitToChordFunctionTest, GDim) {
    auto result = bw_music::fitToChordFunction(m_cMajorChordTrack, bw_music::Chord{bw_music::PitchClass::Value::G, bw_music::ChordType::Value::dim});
    testSimpleTrack(result, 55, 58, 61); // G, A#, C#
}

TEST_F(FitToChordFunctionTest, GMajLowerLimit) {
    auto result = bw_music::fitToChordFunction(m_lowerLimitTrack, bw_music::Chord{bw_music::PitchClass::Value::G, bw_music::ChordType::Value::M});
    testSimpleTrack(result, 7, 11, 2); // G, E, B
}

TEST_F(FitToChordFunctionTest, FMajUpperLimit) {
    auto result = bw_music::fitToChordFunction(m_upperLimitTrack, bw_music::Chord{bw_music::PitchClass::Value::F, bw_music::ChordType::Value::M});
    testSimpleTrack(result, 125, 117, 120); // F, A, C
}
