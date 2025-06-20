#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/appendTrackFunction.hpp>
#include <MusicLib/Processors/concatenateProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ConcatenateProcessorTest, appendFuncSimple) {
    bw_music::TrackBuilder trackBuilderA;
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackBuilderA);
    bw_music::Track trackA = trackBuilderA.finishAndGetTrack();

    bw_music::TrackBuilder trackBuilderB;
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72}, trackBuilderB);
    bw_music::Track trackB = trackBuilderB.finishAndGetTrack();

    appendTrack(trackA, trackB);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackA);
}

TEST(ConcatenateProcessorTest, appendFuncGaps) {
    bw_music::TrackBuilder trackBuilderA;
    const std::vector<testUtils::NoteInfo> noteInfosA{{60, 1, babelwires::Rational(1, 4)},
                                                      {62, 0, babelwires::Rational(1, 4)},
                                                      {64, 0, babelwires::Rational(1, 4)},
                                                      {65, 0, babelwires::Rational(1, 4)}};
    testUtils::addNotes(noteInfosA, trackBuilderA);
    trackBuilderA.setDuration(3);
    bw_music::Track trackA = trackBuilderA.finishAndGetTrack();

    bw_music::TrackBuilder trackBuilderB;
    const std::vector<testUtils::NoteInfo> noteInfosB{
        {67, 1, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)},
        {72, 0, babelwires::Rational(1, 4)},
    };
    testUtils::addNotes(noteInfosB, trackBuilderB);
    trackBuilderB.setDuration(3);
    bw_music::Track trackB = trackBuilderB.finishAndGetTrack();

    appendTrack(trackA, trackB);

    EXPECT_EQ(trackA.getDuration(), 6);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {60, 1, babelwires::Rational(1, 4)}, {62, 0, babelwires::Rational(1, 4)}, {64, 0, babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)}, {67, 2, babelwires::Rational(1, 4)}, {69, 0, babelwires::Rational(1, 4)},
        {71, 0, babelwires::Rational(1, 4)}, {72, 0, babelwires::Rational(1, 4)},
    };

    testUtils::testNotes(expectedNoteInfos, trackA);
}

TEST(ConcatenateProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::ConcatenateProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = bw_music::ConcatenateProcessorInput::Instance(processor.getInput());
    const auto output = bw_music::ConcatenateProcessorOutput::ConstInstance(processor.getOutput());

    ASSERT_EQ(input.getInput().getSize(), 2);
    EXPECT_EQ(input.getInput().getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(input.getInput().getEntry(1).get().getDuration(), 0);

    {
        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, track);
        input.getInput().getEntry(0).set(track.finishAndGetTrack());
    }

    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, output.getOutput().get());

    processor.getInput().clearChanges();
    {
        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72}, track);
        input.getInput().getEntry(1).set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, output.getOutput().get());

    processor.getInput().clearChanges();
    // Insert a new track at position 1.
    {
        input.getInput().setSize(3);
        input.getInput().getEntry(2).set(input.getInput().getEntry(1)->getValue());
        {
            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{67, 65}, track);
            input.getInput().getEntry(1).set(track.finishAndGetTrack());
        }
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 65, 67, 69, 71, 72},
                               output.getOutput().get());
}
