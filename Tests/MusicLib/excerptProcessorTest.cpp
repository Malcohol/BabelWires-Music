#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/excerptFunction.hpp>
#include <MusicLib/Processors/excerptProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Utilities/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(ExcerptProcessorTest, funcSimple) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = bw_music::getTrackExcerpt(trackIn.finishAndGetTrack(), babelwires::Rational(1, 2), 1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcEmptyBefore) {
    bw_music::Track trackIn;
    bw_music::NoteOnEvent note;
    note.m_pitch = 64;
    note.m_velocity = 100;
    note.setTimeSinceLastEvent(10);
    trackIn.addEvent(note);

    auto trackOut = bw_music::getTrackExcerpt(trackIn, 3, 4);

    EXPECT_EQ(trackOut.getDuration(), 4);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyAfter) {
    bw_music::Track trackIn;

    auto trackOut = bw_music::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcEmptyBetween) {
    bw_music::Track trackIn;
    bw_music::NoteOnEvent noteOn;
    noteOn.m_pitch = 64;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    bw_music::NoteOffEvent noteOff;
    noteOff.m_pitch = 64;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(10);
    trackIn.addEvent(noteOff);

    auto trackOut = bw_music::getTrackExcerpt(trackIn, 2, 2);

    EXPECT_EQ(trackOut.getDuration(), 2);
    EXPECT_EQ(trackOut.getNumEvents(), 0);
}

TEST(ExcerptProcessorTest, funcDropSpanningGroup) {
    bw_music::TrackBuilder trackIn;
    bw_music::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);
    bw_music::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOff);

    auto trackOut = bw_music::getTrackExcerpt(trackIn.finishAndGetTrack(), babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcDropInitialGroup) {
    bw_music::TrackBuilder trackIn;

    bw_music::NoteOnEvent noteOn;
    noteOn.m_pitch = 40;
    noteOn.m_velocity = 100;
    noteOn.setTimeSinceLastEvent(1);
    trackIn.addEvent(noteOn);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    bw_music::NoteOffEvent noteOff;
    noteOff.m_pitch = 40;
    noteOff.m_velocity = 100;
    noteOff.setTimeSinceLastEvent(0);
    trackIn.addEvent(noteOff);

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72}, trackIn);

    auto trackOut = bw_music::getTrackExcerpt(trackIn.finishAndGetTrack(), babelwires::Rational(3, 2), 1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69}, trackOut);
}

TEST(ExcerptProcessorTest, funcGaps) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, trackIn);

    auto trackOut = bw_music::getTrackExcerpt(trackIn.finishAndGetTrack(), babelwires::Rational(3, 8), 1);

    const std::vector<testUtils::NoteInfo> expectedNoteInfos{
        {64, babelwires::Rational(1, 8), babelwires::Rational(1, 4)},
        {65, 0, babelwires::Rational(1, 4)},
        {67, 0, babelwires::Rational(1, 4)},
        {69, 0, babelwires::Rational(1, 8)}};

    testUtils::testNotes(expectedNoteInfos, trackOut);
}

TEST(ExcerptProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::ExcerptProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(bw_music::ExcerptProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(bw_music::ExcerptProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(outputArray);

    bw_music::ExcerptProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    in.getDuratn().set(1);

    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(0).get().getNumEvents(), 0);

    processor.getInput().clearChanges();
    {
        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes({60, 62, 64, 65, 67, 69, 71, 72}, track);
        inArray.getEntry(0).set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    in.getStart().set(1);
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72}, outArray.getEntry(0).get());

    //TODO: Would like this test to confirm that adding a track entry does not cause existing entries to be changed.
    // However that functionality didn't survive the switch to types/values.
    processor.getInput().clearChanges();
    {
        inArray.setSize(2);
        {
            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{48, 50, 52, 53, 55, 57, 59, 60}, track);
            inArray.getEntry(1).set(track.finishAndGetTrack());
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray->getNumChildren(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{55, 57, 59, 60}, outArray.getEntry(1).get());

    // Check that input changes to an entry only affect its output entry. For this test, we clear the output changes too.
    processor.getInput().clearChanges();
    processor.getOutput().clearChanges();
    // Set it to empty.
    inArray.getEntry(0).set(bw_music::Track());
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray->getNumChildren(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(0).get().getNumEvents(), 0);

    // The other entry is untouched.
    EXPECT_FALSE(outArray.getEntry(1)->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged));
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{55, 57, 59, 60}, outArray.getEntry(1).get());
}
