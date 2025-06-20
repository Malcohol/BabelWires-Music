#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/quantizeFunction.hpp>
#include <MusicLib/Processors/quantizeProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Utilities/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(QuantizeProcessorTest, funcSimple) {
    bw_music::TrackBuilder trackBuilderIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
            {62, babelwires::Rational(0, 17), babelwires::Rational(10, 17)},
            {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
            {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
        },
        trackBuilderIn);

    bw_music::Track trackIn = trackBuilderIn.finishAndGetTrack();
    {
        auto trackOut = bw_music::quantize(trackIn, babelwires::Rational(1, 2));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(1, 2)},
                              {64, 0, babelwires::Rational(1, 1)},
                              {65, babelwires::Rational(1, 2), babelwires::Rational(1, 1)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = bw_music::quantize(trackIn, babelwires::Rational(1, 4));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(1, 2)},
                              {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                              {65, 0, babelwires::Rational(5, 4)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
    {
        auto trackOut = bw_music::quantize(trackIn, babelwires::Rational(1, 8));
        testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                              {62, 0, babelwires::Rational(5, 8)},
                              {64, 0, babelwires::Rational(9, 8)},
                              {65, 0, babelwires::Rational(5, 4)}},
                             trackOut);
        EXPECT_EQ(trackOut.getDuration(), 4);
    }
}

TEST(QuantizeProcessorTest, funcCollapsedGroup) {
    bw_music::TrackBuilder trackIn;

    testUtils::addNotes(
        {
            {60, babelwires::Rational(1, 17), babelwires::Rational(15, 17)},
            // This note shrinks to nothing and so should be removed.
            {62, babelwires::Rational(0, 17), babelwires::Rational(2, 17)},
            {64, babelwires::Rational(0, 17), babelwires::Rational(16, 17)},
            {65, babelwires::Rational(0, 17), babelwires::Rational(17, 17)},
        },
        trackIn);

    auto trackOut = bw_music::quantize(trackIn.finishAndGetTrack(), babelwires::Rational(1, 8));
    testUtils::testNotes({{60, 0, 1}, {64, 0, 1}, {65, 0, 1}}, trackOut);
}

TEST(QuantizeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::QuantizeProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(bw_music::QuantizeProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(bw_music::QuantizeProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(
        outputArray);

    bw_music::QuantizeProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    {
        in.getBeat().set(babelwires::Rational(1, 8));
        bw_music::TrackBuilder track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(10, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        inArray.getEntry(0).set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(5, 8)},
                          {64, 0, babelwires::Rational(9, 8)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    in.getBeat().set(babelwires::Rational(1, 4));
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(1, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    {
        bw_music::TrackBuilder track;
        testUtils::addNotes(
            {
                {60, babelwires::Rational(1, 17), babelwires::Rational(16, 17)},
                {62, babelwires::Rational(0, 17), babelwires::Rational(27, 17)},
                {64, babelwires::Rational(1, 17), babelwires::Rational(18, 17)},
                {65, babelwires::Rational(1, 17), babelwires::Rational(22, 17)},
            },
            track);
        inArray.getEntry(0).set(track.finishAndGetTrack());
    }
    processor.process(testEnvironment.m_log);

    testUtils::testNotes({{60, 0, babelwires::Rational(1, 1)},
                          {62, 0, babelwires::Rational(3, 2)},
                          {64, babelwires::Rational(1, 4), babelwires::Rational(1, 1)},
                          {65, 0, babelwires::Rational(5, 4)}},
                         outArray.getEntry(0).get());
}
