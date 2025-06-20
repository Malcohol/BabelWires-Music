#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/transposeFunction.hpp>
#include <MusicLib/Processors/transposeProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Utilities/trackValidator.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(TransposeProcessorTest, funcSimpleZero) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), 0);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositive) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), 1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{61, 63, 65, 66}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegative) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), -1);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{59, 61, 63, 64}, trackOut);
}

TEST(TransposeProcessorTest, funcSimplePositiveLimit) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), 127);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{127, 127, 127, 127}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleNegativeLimit) {
    bw_music::TrackBuilder trackIn;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), -127);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{0, 0, 0, 0}, trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsZero) {
    bw_music::TrackBuilder trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), 0);

    testUtils::testChords(
        {
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::aug},
        },
        trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsPositive) {
    bw_music::TrackBuilder trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::B, bw_music::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), 1);

    testUtils::testChords(
        {
            {bw_music::PitchClass::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::F, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::aug},
        },
        trackOut);
}

TEST(TransposeProcessorTest, funcSimpleChordsNegative) {
    bw_music::TrackBuilder trackIn;

    // Some random chords.
    testUtils::addChords(
        {
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::B, bw_music::ChordType::ChordType::Value::aug},
        },
        trackIn);

    auto trackOut = bw_music::transposeTrack(trackIn.finishAndGetTrack(), -1);

    testUtils::testChords(
        {
            {bw_music::PitchClass::PitchClass::Value::B, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::G, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::Ash, bw_music::ChordType::ChordType::Value::aug},
        },
        trackOut);
}

TEST(TransposeProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::TransposeProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(bw_music::TransposeProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(bw_music::TransposeProcessor::getCommonArrayId())
            .is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(
        outputArray);

    bw_music::TransposeProcessorInput::Instance in(input);

    EXPECT_EQ(in.getOffset().get(), 0);

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
    testUtils::testSimpleNotes({60, 62, 64, 65}, outArray.getEntry(0).get());

    processor.getInput().clearChanges();
    in.getOffset().set(1);
    processor.process(testEnvironment.m_log);
    testUtils::testSimpleNotes({61, 63, 65, 66}, outArray.getEntry(0).get());

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
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{61, 63, 65, 66}, outArray.getEntry(0).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{49, 51, 53, 54}, outArray.getEntry(1).get());
}
