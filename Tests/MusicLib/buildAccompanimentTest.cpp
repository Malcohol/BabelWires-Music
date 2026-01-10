#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreePathUtils.hpp>
#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Processors/buildAccompanimentProcessor.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Types/chordTypeSet.hpp>
#include <MusicLib/libRegistration.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Domains/Music/Plugins/TestPlugin/libRegistration.hpp>
#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

namespace {
    /// Register the music lib in the test environment.
    /// This needs to happen before the processor is constructed, below.
    class MusicLibTestFixture : public ::testing::Test {
      public:
        MusicLibTestFixture() {
            bw_music::registerLib(m_testEnv.m_projectContext);
            bw_music_testplugin::registerLib(m_testEnv.m_projectContext);
        }
        testUtils::TestEnvironment m_testEnv;
    };
} // namespace

class BuildAccompanimentTest : public MusicLibTestFixture {
  protected:
    BuildAccompanimentTest()
        : m_processor(m_testEnv.m_projectContext) {}

    void SetUp() override {}

    void instantiateInputTypeVariable(const babelwires::TypeExp& type) {
        const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;
        babelwires::ValueTreeRoot& input = m_processor.getInput();
        babelwires::ValueHolder inputValue = input.getValue();
        const auto& inputType = typeSystem.getRegisteredType<bw_music::BuildAccompanimentProcessorInput>();
        inputType->setTypeVariableAssignmentAndInstantiate(typeSystem, inputValue, {type});
        input.setValue(inputValue);
    }

    void setInputValues(const std::set<bw_music::ChordType::Value>& chordTypes, babelwires::ValueHolder inputTracks) {
        const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;
        babelwires::ValueTreeRoot& input = m_processor.getInput();
        const babelwires::ValueHolder& inputValue = input.getValue();
        const auto& chordType = typeSystem.getRegisteredType<bw_music::ChordTypeSet>();

        input.setDescendentValue(
            babelwires::Path({babelwires::PathStep(babelwires::GenericType::getStepToValue()),
                              babelwires::PathStep(bw_music::BuildAccompanimentProcessorInput::getIdOfChordsArray())}),
            chordType->createValueFromChordTypes(typeSystem, chordTypes));
        input.setDescendentValue(
            babelwires::Path({babelwires::PathStep(babelwires::GenericType::getStepToValue()),
                              babelwires::PathStep(bw_music::BuildAccompanimentProcessorInput::getIdOfInput())}),
            inputTracks);
    }

    const babelwires::ValueTreeNode& getAccompanimentOutput() {
        const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;
        const babelwires::ValueTreeRoot& output = m_processor.getOutput();
        return followPath(
            babelwires::Path({babelwires::PathStep(babelwires::GenericType::getStepToValue()),
                              babelwires::PathStep(bw_music::BuildAccompanimentProcessorOutput::getIdOfResult())}),
            output);
    }

    bw_music::BuildAccompanimentProcessor m_processor;
};

TEST_F(BuildAccompanimentTest, trackTest) {
    const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;

    instantiateInputTypeVariable(bw_music::DefaultTrackType::getThisIdentifier());

    babelwires::ValueHolder track = testUtils::getTrackOfSimpleNotes({60, 62, 64, 67, 71});

    setInputValues({bw_music::ChordType::Value::m, bw_music::ChordType::Value::m7}, track);

    const auto& chordType = typeSystem.getRegisteredType<bw_music::ChordType>();

    m_processor.process(m_testEnv.m_log);

    const babelwires::ValueTreeNode& output = getAccompanimentOutput();
    ASSERT_EQ(output.getNumChildren(), 2);
    {
        unsigned int indexOfMinor = output.getType()->is<babelwires::RecordType>().getChildIndexFromStep(
            output.getValue(), chordType->getIdentifierFromValue(bw_music::ChordType::Value::m));
        ASSERT_NE(indexOfMinor, -1);
        const babelwires::ValueTreeNode& minorNode = *output.getChild(indexOfMinor);
        EXPECT_TRUE(minorNode.getType()->as<bw_music::DefaultTrackType>());
        testUtils::testSimpleNotes({60, 62, 63, 67, 71}, minorNode.getValue()->is<bw_music::Track>());
    }
    {
        unsigned int indexOfMinor7 = output.getType()->is<babelwires::RecordType>().getChildIndexFromStep(
        output.getValue(), chordType->getIdentifierFromValue(bw_music::ChordType::Value::m7));
        ASSERT_NE(indexOfMinor7, -1);
        const babelwires::ValueTreeNode& minor7Node = *output.getChild(indexOfMinor7);
        EXPECT_TRUE(minor7Node.getType()->as<bw_music::DefaultTrackType>());
        testUtils::testSimpleNotes({60, 62, 63, 67, 70}, minor7Node.getValue()->is<bw_music::Track>());
    }
}

TEST_F(BuildAccompanimentTest, testTrackContainerTest) {
    const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;

    instantiateInputTypeVariable(bw_music_testplugin::TestTrackContainer::getThisIdentifier());

    const auto& testTrackContainerType = typeSystem.getRegisteredType<bw_music_testplugin::TestTrackContainer>();
    babelwires::ValueTreeRoot inputTracks(typeSystem, bw_music_testplugin::TestTrackContainer::getThisIdentifier());
    bw_music_testplugin::TestTrackContainer::Instance inputTracksInstance(inputTracks);
    inputTracksInstance.gettrack1().set(testUtils::getTrackOfSimpleNotes({60, 62, 64, 67, 71}));
    inputTracksInstance.gettrack2().set(testUtils::getTrackOfSimpleNotes({71, 67, 64, 62, 60}));
    inputTracksInstance.getother().set(42);

    setInputValues({bw_music::ChordType::Value::m, bw_music::ChordType::Value::m7}, inputTracks.getValue());

    m_processor.process(m_testEnv.m_log);

    const auto& chordType = typeSystem.getRegisteredType<bw_music::ChordType>();

    const babelwires::ValueTreeNode& output = getAccompanimentOutput();
    ASSERT_EQ(output.getNumChildren(), 2);
    {
        unsigned int indexOfMinor = output.getType()->is<babelwires::RecordType>().getChildIndexFromStep(
            output.getValue(), chordType->getIdentifierFromValue(bw_music::ChordType::Value::m));
        ASSERT_NE(indexOfMinor, -1);
        const babelwires::ValueTreeNode& minorNode = *output.getChild(indexOfMinor);
        EXPECT_TRUE(minorNode.getType()->as<bw_music_testplugin::TestTrackContainer>());
        bw_music_testplugin::TestTrackContainer::ConstInstance minorInstance(minorNode);
        testUtils::testSimpleNotes({60, 62, 63, 67, 71}, minorInstance.gettrack1().get());
        testUtils::testSimpleNotes({71, 67, 63, 62, 60}, minorInstance.gettrack2().get());
        EXPECT_EQ(minorInstance.getother().get(), 42);
    }
    {
        unsigned int indexOfMinor7 = output.getType()->is<babelwires::RecordType>().getChildIndexFromStep(
        output.getValue(), chordType->getIdentifierFromValue(bw_music::ChordType::Value::m7));
        ASSERT_NE(indexOfMinor7, -1);
        const babelwires::ValueTreeNode& minor7Node = *output.getChild(indexOfMinor7);
        EXPECT_TRUE(minor7Node.getType()->as<bw_music_testplugin::TestTrackContainer>());
        bw_music_testplugin::TestTrackContainer::ConstInstance minor7Instance(minor7Node);
        testUtils::testSimpleNotes({60, 62, 63, 67, 70}, minor7Instance.gettrack1().get());
        testUtils::testSimpleNotes({70, 67, 63, 62, 60}, minor7Instance.gettrack2().get());
        EXPECT_EQ(minor7Instance.getother().get(), 42);
    }
}
