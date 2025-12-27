#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/mergeFunction.hpp>
#include <MusicLib/Functions/splitAtPitchFunction.hpp>
#include <MusicLib/Processors/splitAtPitchProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SplitAtPitchProcessorTest, monophonicSplit) {
    bw_music::TrackBuilder track;
    testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);

    bw_music::SplitAtPitchResult result = bw_music::splitAtPitch(67, track.finishAndGetTrack());

    const std::vector<testUtils::NoteInfo> expectedNotesAbove{
        {67, babelwires::Rational(1, 4), 1},
        {69, babelwires::Rational(1, 4)},
        {71, babelwires::Rational(1, 4)},
        {72, babelwires::Rational(1, 4)},
    };
    const std::vector<testUtils::NoteInfo> expectedNotesBelow{
        {60, babelwires::Rational(1, 4)},
        {62, babelwires::Rational(1, 4)},
        {64, babelwires::Rational(1, 4)},
        {65, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, result.m_equalOrAbove);
    EXPECT_EQ(result.m_equalOrAbove.getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, result.m_below);
    EXPECT_EQ(result.m_below.getDuration(), 2);
    EXPECT_EQ(result.m_other.getNumEvents(), 0);
    EXPECT_EQ(result.m_other.getDuration(), 2);
}

TEST(SplitAtPitchProcessorTest, aboveAndBelowSplit) {
    bw_music::TrackBuilder trackBuilder;
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 72});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 48});
    trackBuilder.addEvent(bw_music::ChordOnEvent{
        0, {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}});
    trackBuilder.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
    trackBuilder.addEvent(bw_music::NoteOffEvent{0, 48});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 74});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 50});
    trackBuilder.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74});
    trackBuilder.addEvent(bw_music::NoteOffEvent{0, 50});
    trackBuilder.addEvent(bw_music::ChordOffEvent{0});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 76});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 52});
    trackBuilder.addEvent(bw_music::ChordOnEvent{
        0, {bw_music::PitchClass::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}});
    trackBuilder.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 76});
    trackBuilder.addEvent(bw_music::NoteOffEvent{0, 52});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 77});
    trackBuilder.addEvent(bw_music::NoteOnEvent{0, 53});
    trackBuilder.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 77});
    trackBuilder.addEvent(bw_music::NoteOffEvent{0, 53});
    trackBuilder.addEvent(bw_music::ChordOffEvent{0});

    bw_music::Track track = trackBuilder.finishAndGetTrack();
    ASSERT_EQ(track.getDuration(), 1);

    bw_music::SplitAtPitchResult result = bw_music::splitAtPitch(60, track);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_equalOrAbove);
    EXPECT_EQ(result.m_equalOrAbove.getDuration(), 1);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_below);
    EXPECT_EQ(result.m_below.getDuration(), 1);
    testUtils::testChords({{bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          result.m_other);
    EXPECT_EQ(result.m_other.getDuration(), 1);
}

TEST(SplitAtPitchProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::SplitAtPitchProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = bw_music::SplitAtPitchProcessorInput::Instance(processor.getInput());
    const auto output = bw_music::SplitAtPitchProcessorOutput::ConstInstance(processor.getOutput());

    input.getPitch().set(babelwires::EnumValue(input.getPitch().getInstanceType().getIdentifierFromIndex(67)));
    {
        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        input.getInput().set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    std::vector<testUtils::NoteInfo> expectedNotesAbove{
        {67, babelwires::Rational(1, 4), 1},
        {69, babelwires::Rational(1, 4)},
        {71, babelwires::Rational(1, 4)},
        {72, babelwires::Rational(1, 4)},
    };
    std::vector<testUtils::NoteInfo> expectedNotesBelow{
        {60, babelwires::Rational(1, 4)},
        {62, babelwires::Rational(1, 4)},
        {64, babelwires::Rational(1, 4)},
        {65, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, output.getAbove().get());
    EXPECT_EQ(output.getAbove().get().getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, output.getBelow().get());
    EXPECT_EQ(output.getBelow().get().getDuration(), 2);
    EXPECT_EQ(output.getOther().get().getNumEvents(), 0);
    EXPECT_EQ(output.getOther().get().getDuration(), 2);

    processor.getInput().clearChanges();
    input.getPitch().set(babelwires::EnumValue(input.getPitch().getInstanceType().getIdentifierFromIndex(64)));
    processor.process(testEnvironment.m_log);

    expectedNotesAbove = {
        {64, babelwires::Rational(1, 4), babelwires::Rational(1, 2)},
        {65, babelwires::Rational(1, 4)},
        {67, babelwires::Rational(1, 4)},
        {69, babelwires::Rational(1, 4)},
        {71, babelwires::Rational(1, 4)},
        {72, babelwires::Rational(1, 4)},
    };
    expectedNotesBelow = {
        {60, babelwires::Rational(1, 4)},
        {62, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNotesAbove, output.getAbove().get());
    EXPECT_EQ(output.getAbove().get().getDuration(), 2);
    testUtils::testNotes(expectedNotesBelow, output.getBelow().get());
    EXPECT_EQ(output.getBelow().get().getDuration(), 2);
    EXPECT_EQ(output.getOther().get().getNumEvents(), 0);
    EXPECT_EQ(output.getOther().get().getDuration(), 2);
}
