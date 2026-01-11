#include <gtest/gtest.h>

#include <MusicLib/Functions/getChordTypesFunction.hpp>
#include <MusicLib/Processors/getChordTypesProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/chordTypeSet.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

class GetChordTypesProcessorTest : public ::testing::Test {
  protected:
    bw_music::Track getTrackOfChords() {
        return testUtils::getTrackOfChords({
            {bw_music::PitchClass::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
            {bw_music::PitchClass::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
            {bw_music::PitchClass::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::dim},
            {bw_music::PitchClass::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::aug},
            {bw_music::PitchClass::PitchClass::Value::G, bw_music::ChordType::ChordType::Value::M},
        });
    }

    void testSetOfChords(std::set<bw_music::ChordType::Value> chordTypes) {
        EXPECT_EQ(chordTypes.size(), 4);
        EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::M), chordTypes.end());
        EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::m), chordTypes.end());
        EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::dim), chordTypes.end());
        EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::aug), chordTypes.end());
    }
};

TEST_F(GetChordTypesProcessorTest, functionBasic) {
    auto chordTypes = bw_music::getChordTypesFunction(getTrackOfChords());
    testSetOfChords(chordTypes);
}

TEST_F(GetChordTypesProcessorTest, functionEmpty) {
    auto chordTypes = bw_music::getChordTypesFunction(bw_music::Track());

    EXPECT_EQ(chordTypes.size(), 0);
}

TEST_F(GetChordTypesProcessorTest, functionNoChords) {
    auto noChords = testUtils::getTrackOfSimpleNotes({60, 62, 64, 67, 71});

    auto chordTypes = bw_music::getChordTypesFunction(noChords);

    EXPECT_EQ(chordTypes.size(), 0);
}

TEST_F(GetChordTypesProcessorTest, processorBasic) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::GetChordTypesProcessor processor(testEnvironment.m_projectContext);

    bw_music::GetChordTypesProcessorInput::Instance input(processor.getInput());
    const bw_music::GetChordTypesProcessorOutput::ConstInstance output(processor.getOutput());

    input.getTrack().set(getTrackOfChords());

    processor.process(testEnvironment.m_log);

    auto chordTypeSetType = output.getChords()->getType()->tryAs<bw_music::ChordTypeSet>();
    EXPECT_NE(chordTypeSetType, nullptr);

    auto chordTypes = chordTypeSetType->getChordTypesFromValue(
        testEnvironment.m_typeSystem, output.getChords()->getValue());

    testSetOfChords(chordTypes);
}
