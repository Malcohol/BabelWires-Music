#include <gtest/gtest.h>

#include <Smf/Percussion/gm2StandardPercussionSet.hpp>
#include <Smf/libRegistration.hpp>
#include <Smf/smfParser.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/pitchBendEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/volumeEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/sustainEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/tempoEvent.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Types/Track/TrackEvents/expressionEvent.hpp>

#include <BaseLib/IO/fileDataSource.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/resultTestUtils.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

TEST(SmfTestSuiteTest, loadAllTestFilesWithoutCrashing) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    int numFilesTested = 0;
    int numSucceeded = 0;
    int numFailed = 0;

    for (auto& p : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (p.path().extension() == ".mid") {
            ++numFilesTested;
            BW_ASSERT_RESULT_ASSIGN(auto midiFile, babelwires::FileDataSource::open(p.path()));
            ON_ERROR(midiFile.closeOnError());
            // This test is not testing that the files are correctly parsed, just that they don't cause crashes or
            // exceptions.
            const auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
            if (result.has_value()) {
                ++numSucceeded;
            } else {
                ++numFailed;
            }
            midiFile.close();
        }
    }

    EXPECT_GT(numFilesTested, 0);
    EXPECT_GT(numSucceeded, 0);
    EXPECT_GT(numFailed, 0);
    EXPECT_EQ(numFilesTested, numSucceeded + numFailed);
}

namespace {
    const char* channelNames[] = {"ch0", "ch1", "ch2"};
} // namespace

TEST(SmfTestSuiteTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"C Major Scale Test");
    ASSERT_TRUE(metadata.tryGetCopyR().has_value());
    EXPECT_EQ(metadata.tryGetCopyR()->get(), u8"https://jazz-soft.net");

    auto tracks = smfSequence.getTrcks0();
    EXPECT_EQ(tracks->getNumChildren(), 1);

    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0);

    const bw_music::Track& track = track0->get();
    const auto& categoryMap = track.getNumEventGroupsByCategory();
    EXPECT_NE(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory()), categoryMap.end());
    EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory())->second, 8);
    EXPECT_EQ(track.getDuration(), babelwires::Rational(1, 4) * 8);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, track);
}

TEST(SmfTestSuiteTest, multichannelChords0) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Multi-channel chords Test 0");

    auto tracks = smfSequence.getTrcks0();
    EXPECT_EQ(tracks->getNumChildren(), 3);

    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67, 69, 71, 72}, tracks.getTrack(0).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{64, 65, 67, 69, 71, 72, 74, 76}, tracks.getTrack(1).get());
    testUtils::testSimpleNotes(std::vector<bw_music::Pitch>{67, 69, 71, 72, 74, 76, 77, 79}, tracks.getTrack(2).get());
}

TEST(SmfTestSuiteTest, multichannelChords1) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Multi-channel chords Test 1");

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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Multi-channel chords Test 2");

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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Multi-channel chords Test 3");

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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    // All these files should be interpreted as having the same sequence of tempo events.
    // The SMF plugin will make those events available in a canonical way via the global track.
    const char* fileNames[] = {
        "test-tempo-events-track-smf0.mid",
        "test-tempo-events-track-smf1.mid",
        "test-tempo-events-track-smf1-nonstandard.mid",
        "test-tempo-events-track-smf1-bad.mid"
    };

    const int expectedBpms[] = {100, 120, 140, 160};
    const babelwires::Rational expectedDeltaTimes[] = {0, 1, 1, 1};

    for (const auto& fileName : fileNames) {
        SCOPED_TRACE(fileName);

        auto midiFileResult = babelwires::FileDataSource::open(fileName);
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);

        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_TRUE(midiFile.close().has_value());
        ASSERT_TRUE(result.has_value());
        const auto& feature = *result;

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};

        const auto& metadata = smfSequence.getMeta();
        ASSERT_TRUE(metadata.tryGetITempo().has_value());
        EXPECT_EQ(metadata.tryGetITempo()->get(), babelwires::Fixed(10000, 2));

        const auto& globalTrack = smfSequence.getGlobal().get();
        auto [tempoIt, tempoEnd] = bw_music::iterateOver<bw_music::TempoEvent>(globalTrack);

        for (int i = 0; i < 4; ++i) {
            ASSERT_NE(tempoIt, tempoEnd);
            // Not all of the tempo values are perfectly represented in MIDI tempo storage.
            EXPECT_EQ(tempoIt->getTempoValue().getBpmRounded(2), expectedBpms[i]);
            EXPECT_EQ(tempoIt->getTimeSinceLastEvent(), expectedDeltaTimes[i]);
            ++tempoIt;
        }
        EXPECT_EQ(tempoIt, tempoEnd);
    }
}

TEST(SmfTestSuiteTest, corruptFiles) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
        midiFile.closeOnError();
        EXPECT_FALSE(result.has_value());
    }
}

TEST(SmfTestSuiteTest, sustainEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-control-40-damper.mid");
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

    const bw_music::Track& track = track0->get();
    auto [sustainIt, sustainEnd] = bw_music::iterateOver<bw_music::SustainEvent>(track);

    // From test-control-40-damper.js at division 96:
    // first sustain: 4 * tick(96) + tick(480) = 864 ticks = 9/4;
    // second sustain delta: 4 * tick(96) + tick(192) = 576 ticks = 3/2.

    ASSERT_NE(sustainIt, sustainEnd);
    EXPECT_TRUE(sustainIt->isSustainOn());
    EXPECT_EQ(sustainIt->getSustainStorage().getUnsigned<7>(), 127);
    EXPECT_EQ(sustainIt->getTimeSinceLastEvent(), babelwires::Rational(9, 4));
    ++sustainIt;

    ASSERT_NE(sustainIt, sustainEnd);
    EXPECT_FALSE(sustainIt->isSustainOn());
    EXPECT_EQ(sustainIt->getSustainStorage().getUnsigned<7>(), 0);
    EXPECT_EQ(sustainIt->getTimeSinceLastEvent(), babelwires::Rational(3, 2));
    ++sustainIt;

    EXPECT_EQ(sustainIt, sustainEnd);
}

TEST(SmfTestSuiteTest, volumeEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-control-07-volume.mid");
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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Control 0x07 Volume Test");

    auto tracks = smfSequence.getTrcks0();
    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0.has_value());

    const bw_music::Track& track = track0->get();
    auto [volumeIt, volumeEnd] = bw_music::iterateOver<bw_music::VolumeEvent>(track);

    ASSERT_NE(volumeIt, volumeEnd);
    EXPECT_EQ(volumeIt->getVolumeStorage().getUnsigned<7>(), 64);
    EXPECT_EQ(volumeIt->getTimeSinceLastEvent(), babelwires::Rational(1, 2));
    ++volumeIt;

    ASSERT_NE(volumeIt, volumeEnd);
    EXPECT_EQ(volumeIt->getVolumeStorage().getUnsigned<7>(), 127);
    EXPECT_EQ(volumeIt->getTimeSinceLastEvent(), babelwires::Rational(1, 2));
    ++volumeIt;

    EXPECT_EQ(volumeIt, volumeEnd);
}

TEST(SmfTestSuiteTest, expressionCourseEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-control-0b-expression-coarse.mid");
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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Control 0x0B Expression Coarse Test");

    auto tracks = smfSequence.getTrcks0();
    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0.has_value());

    const bw_music::Track& track = track0->get();
    auto [expressionBegin, expressionEnd] = bw_music::iterateOver<bw_music::ExpressionEvent>(track);

    int numExpressionEvents = 0;
    std::uint64_t minExpressionValue = 0x3fffu;
    std::uint64_t maxExpressionValue = 0;
    for (auto it = expressionBegin; it != expressionEnd; ++it) {
        const std::uint64_t value14 = it->getExpressionStorage().getUnsigned<14>();
        minExpressionValue = std::min(minExpressionValue, value14);
        maxExpressionValue = std::max(maxExpressionValue, value14);
        ++numExpressionEvents;
    }

    EXPECT_EQ(numExpressionEvents, 129);
    EXPECT_EQ(minExpressionValue, 0);
    EXPECT_EQ(maxExpressionValue, 0x3fffu);
}

TEST(SmfTestSuiteTest, expressionFineEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-control-0b-expression-fine.mid");
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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Control 0x0B Expression Fine Test");

    auto tracks = smfSequence.getTrcks0();
    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0.has_value());

    const bw_music::Track& track = track0->get();
    auto [expressionBegin, expressionEnd] = bw_music::iterateOver<bw_music::ExpressionEvent>(track);

    int numExpressionEvents = 0;
    int numFineResolutionEvents = 0;
    std::uint64_t minExpressionValue = 0x3fffu;
    std::uint64_t maxExpressionValue = 0;
    for (auto it = expressionBegin; it != expressionEnd; ++it) {
        const std::uint64_t value14 = it->getExpressionStorage().getUnsigned<14>();
        if ((value14 % 128) != 0) {
            ++numFineResolutionEvents;
        }
        minExpressionValue = std::min(minExpressionValue, value14);
        maxExpressionValue = std::max(maxExpressionValue, value14);
        ++numExpressionEvents;
    }

    EXPECT_EQ(numExpressionEvents, 257);
    EXPECT_GT(numFineResolutionEvents, 128);
    EXPECT_EQ(minExpressionValue, 0);
    EXPECT_EQ(maxExpressionValue, 0x3fffu);
}

TEST(SmfTestSuiteTest, expressionStaggeredEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-control-0b-expression-staggered.mid");
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
    EXPECT_EQ(metadata.tryGetName()->get(), u8"Control 0x0B Expression Staggered Test");

    auto tracks = smfSequence.getTrcks0();
    auto track0 = tracks.tryGetTrack(0);
    ASSERT_TRUE(track0.has_value());

    const bw_music::Track& track = track0->get();
    auto [expressionBegin, expressionEnd] = bw_music::iterateOver<bw_music::ExpressionEvent>(track);

    int numExpressionEvents = 0;
    int numFineResolutionEvents = 0;
    std::uint64_t minExpressionValue = 0x3fffu;
    std::uint64_t maxExpressionValue = 0;
    for (auto it = expressionBegin; it != expressionEnd; ++it) {
        const std::uint64_t value14 = it->getExpressionStorage().getUnsigned<14>();
        if ((value14 % 128) != 0) {
            ++numFineResolutionEvents;
        }
        minExpressionValue = std::min(minExpressionValue, value14);
        maxExpressionValue = std::max(maxExpressionValue, value14);
        ++numExpressionEvents;
    }

    EXPECT_EQ(numExpressionEvents, 257);
    EXPECT_GT(numFineResolutionEvents, 128);
    EXPECT_EQ(minExpressionValue, 0);
    EXPECT_EQ(maxExpressionValue, 0x3fffu);
}

TEST(SmfTestSuiteTest, pitchBend) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    auto midiFileResult = babelwires::FileDataSource::open("test-rpn-00-00-pitch-bend-range.mid");
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

    const bw_music::Track& track = track0->get();
    auto [pitchBendBegin, pitchBendEnd] = bw_music::iterateOver<bw_music::PitchBendEvent>(track);

    int numPitchBendEvents = 0;
    std::uint16_t minPitchBendValue = 0x3fffu;
    std::uint16_t maxPitchBendValue = 0;
    for (auto it = pitchBendBegin; it != pitchBendEnd; ++it) {
        const auto value = static_cast<std::uint16_t>(it->getPitchBendStorage().getUnsigned<14>());
        minPitchBendValue = std::min(minPitchBendValue, value);
        maxPitchBendValue = std::max(maxPitchBendValue, value);
        ++numPitchBendEvents;
    }

    // The JS generator emits five bend() sections with 192 + 384 + 192 bends each.
    EXPECT_EQ(numPitchBendEvents, 5 * (192 + 384 + 192));
    EXPECT_EQ(minPitchBendValue, 0);
    EXPECT_EQ(maxPitchBendValue, 0x3fffu);
}

TEST(SmfTestSuiteTest, testAllGMPercussion) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
    EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory()), categoryMap.end());
    EXPECT_NE(categoryMap.find(bw_music::PercussionEvent::getPercussionEventCategory()), categoryMap.end());

    const auto& gm2StandardPercussionSet =
        testEnvironment.m_typeSystem.getRegisteredType<smf::GM2StandardPercussionSet>();

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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

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
        EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory()), categoryMap.end());
        ASSERT_NE(categoryMap.find(bw_music::PercussionEvent::getPercussionEventCategory()), categoryMap.end());
        EXPECT_EQ(categoryMap.find(bw_music::PercussionEvent::getPercussionEventCategory())->second, 4);
    }
    {
        const bw_music::Track& track = track9->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(bw_music::PercussionEvent::getPercussionEventCategory()), categoryMap.end());
        ASSERT_NE(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory()), categoryMap.end());
        EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::getNoteEventCategory())->second, 4);
    }
}
