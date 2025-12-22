#include <gtest/gtest.h>

#include <MusicLib/Functions/accompanimentSequencerFunction.hpp>
#include <MusicLib/Processors/accompanimentSequencerProcessor.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>

#include <Domains/Music/Plugins/TestPlugin/libRegistration.hpp>
#include <Domains/Music/Plugins/TestPlugin/testTrackContainer.hpp>
#include <Domains/Music/Plugins/TestPlugin/simpleAccompaniment.hpp>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

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

class AccompanimentSequencerTest : public MusicLibTestFixture {
  protected:
    AccompanimentSequencerTest()
        : m_processor(m_testEnv.m_projectContext) {}

    void SetUp() override {
        const auto& typeSystem = m_testEnv.m_projectContext.m_typeSystem;
        babelwires::ValueTreeRoot& input = m_processor.getInput();
        babelwires::ValueHolder inputValue = input.getValue();
        const auto& inputType = typeSystem.getEntryByType<bw_music::AccompanimentSequencerProcessorInput>();
        inputType.setTypeVariableAssignmentAndInstantiate(typeSystem, inputValue, {bw_music_testplugin::SimpleAccompaniment::getThisType()});
        input.setValue(inputValue);
    }

    void setChordTrack(bw_music::Track chordTrack) {
        babelwires::ValueTreeRoot& input = m_processor.getInput();
        input.setDescendentValue(
            babelwires::Path({babelwires::PathStep(babelwires::GenericType::getStepToValue()),
                              babelwires::PathStep(bw_music::AccompanimentSequencerProcessorInput::getChordTrackId())}),
            std::move(chordTrack));
    }

    bw_music::Track getResultTrack(int i) {
        const babelwires::ValueTreeRoot& output = m_processor.getOutput();
        babelwires::Path pathToTrack({babelwires::GenericType::getStepToValue(),
                              bw_music::AccompanimentSequencerProcessorOutput::getResultId()});
        pathToTrack.pushStep((i == 0) ? bw_music_testplugin::TestTrackContainer::getTrack1Id()
                                : bw_music_testplugin::TestTrackContainer::getTrack2Id());
        return babelwires::followPath(pathToTrack, output).getValue()->is<bw_music::Track>();
    }

    bw_music::AccompanimentSequencerProcessor m_processor;
};

TEST_F(AccompanimentSequencerTest, SimpleContiguousChords) {
    // Set a chord track
    bw_music::Track chordTrack = testUtils::getTrackOfChords({
        {bw_music::PitchClass::Value::C, bw_music::ChordType::Value::M, bw_music::ModelDuration(2)},
        {bw_music::PitchClass::Value::F, bw_music::ChordType::Value::m, bw_music::ModelDuration(2)},
        {bw_music::PitchClass::Value::G, bw_music::ChordType::Value::dim, bw_music::ModelDuration(2)}
    });
    setChordTrack(std::move(chordTrack));

    // Process
    m_processor.process(m_testEnv.m_log);

    // Get the result tracks
    {
        bw_music::Track resultTrack1 = getResultTrack(0);
        EXPECT_EQ(resultTrack1.getNumEvents(), bw_music_testplugin::SimpleAccompaniment::getNumEventsInTrack() * 2 * 3);
        EXPECT_EQ(resultTrack1.getDuration(), chordTrack.getDuration());
        testUtils::testNotes({
            {60}, {64}, {67}, {72}, {60}, {64}, {67}, {72},
            {65}, {68}, {72}, {77}, {65}, {68}, {72}, {77},
            {55}, {58}, {61}, {67}, {55}, {58}, {61}, {67}
        }, resultTrack1);
    }
    {
        bw_music::Track resultTrack2 = getResultTrack(1);
        EXPECT_EQ(resultTrack2.getNumEvents(), bw_music_testplugin::SimpleAccompaniment::getNumEventsInTrack() * 2 * 3);
        EXPECT_EQ(resultTrack2.getDuration(), chordTrack.getDuration());
        testUtils::testNotes({
            {72}, {76}, {79}, {84}, {72}, {76}, {79}, {84},
            {77}, {80}, {84}, {89}, {77}, {80}, {84}, {89},
            {67}, {70}, {73}, {79}, {67}, {70}, {73}, {79}
        }, resultTrack2);
    }
}
