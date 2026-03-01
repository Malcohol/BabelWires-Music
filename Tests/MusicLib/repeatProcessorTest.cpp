#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/repeatFunction.hpp>
#include <MusicLib/Processors/repeatProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(RepeatProcessorTest, funcSimpleZero) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::repeatTrack(trackIn.finishAndGetTrack(), 0);

    EXPECT_EQ(trackOut.getNumEvents(), 0);
    EXPECT_EQ(trackOut.getDuration(), 0);
}

TEST(RepeatProcessorTest, funcSimpleOnce) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::repeatTrack(trackIn.finishAndGetTrack(), 1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackOut);
}

TEST(RepeatProcessorTest, funcSimpleTwice) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::repeatTrack(trackIn.finishAndGetTrack(), 2);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 60, 62, 64, 65}, trackOut);
}

TEST(RepeatProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::RepeatProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.assertGetChildFromStep(bw_music::RepeatProcessor::getCommonArrayId());
    const babelwires::ValueTreeNode& outputArray =
        output.assertGetChildFromStep(bw_music::RepeatProcessor::getCommonArrayId());

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(
        outputArray);

    bw_music::RepeatProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    {
        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes({60, 62, 64, 65}, track);
        inArray.getEntry(0).set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 60, 62, 64, 65},
                               outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    in.getCount().set(1);
    processor.process(testEnvironment.m_log);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        inArray.setSize(2);
        {
            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{48, 50, 52, 53}, track);
            inArray.getEntry(1).set(track.finishAndGetTrack());
        }
    }
    processor.process(testEnvironment.m_log);

    ASSERT_EQ(outArray.getSize(), 2);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 1);
    EXPECT_EQ(outArray.getEntry(1).get().getDuration(), 1);
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{48, 50, 52, 53}, outArray.getEntry(1).get());
}