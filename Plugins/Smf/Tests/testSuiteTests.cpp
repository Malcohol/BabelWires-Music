#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BaseLib/IO/fileDataSource.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SmfTestSuiteTest, loadAllTestFilesWithoutCrashing) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    int numFilesTested = 0;

    for (auto& p : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (p.path().extension() == ".mid") {
            ++numFilesTested;
            try {
                auto midiFileResult = babelwires::FileDataSource::open(p.path());
                ASSERT_TRUE(midiFileResult.has_value());
                auto midiFile = std::move(*midiFileResult);
                auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
                midiFile.close();
            } catch (...) {
                EXPECT_TRUE(false);
            }
        }
    }

    EXPECT_GT(numFilesTested, 0);
}

namespace {
    const char* channelNames[] = {"ch0", "ch1", "ch2"};
} // namespace

TEST(SmfTestSuiteTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-c-major-scale.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "C Major Scale Test");
    ASSERT_TRUE(metadata.tryGetCopyR().has_value());
    EXPECT_EQ(metadata.tryGetCopyR()->get(), "https://jazz-soft.net");

    auto tracks = smfSequence.getTrcks0();
    EXPECT_EQ(tracks->getNumChildren(), 1);

    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0);

    const bw_music::Track& track = track0->get();
    const auto& categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_NE(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
    EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory)->second, 8);
    EXPECT_EQ(track.getDuration(), babelwires::Rational(1, 4) * 8);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track);
}

TEST(SmfTestSuiteTest, multichannelChords0) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-multichannel-chords-0.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 0");

    auto tracks = smfSequence.getTrcks0();
    EXPECT_EQ(tracks->getNumChildren(), 3);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks.getTrack(0).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks.getTrack(1).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks.getTrack(2).get());
}

TEST(SmfTestSuiteTest, multichannelChords1) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-multichannel-chords-1.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 1");

    auto tracks = smfSequence.getTrcks1();
    ASSERT_EQ(tracks.getSize(), 3);

    auto track0 = tracks.getEntry(0);
    EXPECT_EQ(track0.getChan().get(), 0);
    auto track1 = tracks.getEntry(1);
    EXPECT_EQ(track1.getChan().get(), 1);
    auto track2 = tracks.getEntry(2);
    EXPECT_EQ(track2.getChan().get(), 2);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track0.getTrack().get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, track1.getTrack().get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, track2.getTrack().get());
}

TEST(SmfTestSuiteTest, multichannelChords2) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-multichannel-chords-2.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 2");

    auto tracks = smfSequence.getTrcks1();
    ASSERT_EQ(tracks.getSize(), 2);

    const auto& track0 = tracks.getEntry(0);
    EXPECT_EQ(track0.getChan().get(), 0);

    const auto& exCh1 = track0.tryGetTrack(1);
    ASSERT_TRUE(exCh1);

    const auto& track1 = tracks.getEntry(1);
    EXPECT_EQ(track1.getChan().get(), 2);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track0.getTrack().get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, exCh1->get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, track1.getTrack().get());
}

TEST(SmfTestSuiteTest, multichannelChords3) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-multichannel-chords-3.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

    auto tracks = smfSequence.getTrcks1();
    ASSERT_EQ(tracks.getSize(), 3);

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Multi-channel chords Test 3");

    const int expectedChannelMapping[] = {0, 1, 0};

    const auto& track0 = tracks.getEntry(0);
    EXPECT_EQ(track0.getChan().get(), 0);

    const auto& track1 = tracks.getEntry(1);
    EXPECT_EQ(track1.getChan().get(), 1);

    const auto& track2 = tracks.getEntry(2);
    EXPECT_EQ(track2.getChan().get(), 0);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track0.getTrack().get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, track1.getTrack().get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, track2.getTrack().get());
}

TEST(SmfTestSuiteTest, trackLength) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-track-length.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;
    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

    auto tracks = smfSequence.getTrcks0();
    EXPECT_EQ(tracks->getNumChildren(), 1);

    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0);

    const bw_music::Track& track = track0->get();
    EXPECT_EQ(track.getDuration(), babelwires::Rational(3, 4));
}

TEST(SmfTestSuiteTest, tempoTest) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-karaoke-kar.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};

    const auto& metadata = smfSequence.getMeta();
    ASSERT_TRUE(metadata.tryGetName().has_value());
    EXPECT_EQ(metadata.tryGetName()->get(), "Karaoke .KAR Test");
    ASSERT_TRUE(metadata.tryGetTempo().has_value());
    EXPECT_EQ(metadata.tryGetTempo()->get(), 90);
}

TEST(SmfTestSuiteTest, corruptFiles) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    {
        auto midiFileResult = babelwires::FileDataSource::open("test-corrupt-file-extra-byte.mid");
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);
        // This is OK.
        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        EXPECT_TRUE(midiFile.close().has_value());
        EXPECT_TRUE(result.has_value());
    }
    {
        auto midiFileResult = babelwires::FileDataSource::open("test-corrupt-file-missing-byte.mid");
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);
        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        EXPECT_TRUE(midiFile.close(babelwires::ErrorState::Error).has_value());
        EXPECT_FALSE(result.has_value());
    }
}

TEST(SmfTestSuiteTest, testAllGMPercussion) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-all-gm-percussion.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

    auto tracks = smfSequence.getTrcks0();
    auto track9 = tracks.tryGetTrack(9);
    ASSERT_TRUE(track9.has_value());

    const bw_music::Track& track = track9->get();

    auto categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
    EXPECT_NE(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());

    const auto& gm2StandardPercussionSet = testEnvironment.m_typeSystem.getRegisteredType<smf::GM2StandardPercussionSet>();

    // The file has each percussion instrument playing three times.
    const auto& percussionInstruments = gm2StandardPercussionSet->getValueSet();
    int eventIndex = 0;
    for (const auto& percussionEvent : bw_music::iterateOver<bw_music::PercussionEvent>(track)) {
        const int instrumentIndex = eventIndex / 6;
        EXPECT_EQ(percussionEvent.getInstrument(),
                  gm2StandardPercussionSet->tryGetInstrumentFromPitch(instrumentIndex + 27));
        if (eventIndex % 2 == 0) {
            EXPECT_TRUE(percussionEvent.tryAs<bw_music::PercussionOnEvent>());
        } else {
            EXPECT_TRUE(percussionEvent.tryAs<bw_music::PercussionOffEvent>());
        }
        ++eventIndex;
    }
}

TEST(SmfTestSuiteTest, testGSDrumPartChange) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    auto midiFileResult = babelwires::FileDataSource::open("test-sysex-gs-40-1x-15-drum-part-change.mid");
    ASSERT_TRUE(midiFileResult.has_value());
    auto midiFile = std::move(*midiFileResult);

    auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
    ASSERT_TRUE(midiFile.close().has_value());
    ASSERT_TRUE(result.has_value());
    const auto& feature = *result;

    smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

    auto tracks = smfSequence.getTrcks0();

    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0.has_value());

    auto track9 = tracks.tryGetTrack(9);
    ASSERT_TRUE(track9.has_value());
    {
        const bw_music::Track& track = track0->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
        ASSERT_NE(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());
        EXPECT_EQ(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory)->second, 4);
    }
    {
        const bw_music::Track& track = track9->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());
        ASSERT_NE(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
        EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory)->second, 4);
    }
}
