#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/Types/File/fileTypeT.hpp>

#include <BaseLib/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/equalSets.hpp>
#include <Tests/TestUtils/tempFilePath.hpp>

namespace {
    struct PercussionTestData {
        smf::GMSpecType::Value m_specificationId;
        babelwires::ShortId m_instrumentId0;
        babelwires::ShortId m_instrumentId1;
        babelwires::ShortId m_instrumentId2;
    };
} // namespace

class SmfStandardPercussionTest : public testing::TestWithParam<PercussionTestData> {};

TEST_P(SmfStandardPercussionTest, saveLoad) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const PercussionTestData& testData = GetParam();

    testUtils::TempFilePath tempFile("standardPercussion.mid",
                                     smf::GMSpecType::getIdentifierFromValue(testData.m_specificationId).toString());

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.m_typeSystem, babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier());
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        smfType.getMeta().getSpec().set(testData.m_specificationId);

        auto track9 = smfType.getTrcks0().activateAndGetTrack(9);

        bw_music::TrackBuilder track;

        track.addEvent(bw_music::PercussionOnEvent{0, testData.m_instrumentId0});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId0});
        track.addEvent(bw_music::PercussionOnEvent{0, testData.m_instrumentId1});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId1});
        track.addEvent(bw_music::PercussionOnEvent{0, testData.m_instrumentId2});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 4), testData.m_instrumentId2});

        track9.set(track.finishAndGetTrack());

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
        ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

        auto tracks = smfSequence.getTrcks0();
        EXPECT_EQ(tracks->getNumChildren(), 1);

        auto track9 = tracks.tryGetTrack(9);
        ASSERT_TRUE(track9);

        const auto& track = track9->get();

        auto categoryMap = track.getNumEventGroupsByCategory();
        EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
        EXPECT_NE(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());

        auto span = bw_music::iterateOver<bw_music::PercussionEvent>(track);

        auto it = span.begin();
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId0);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId0);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId1);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId1);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOnEvent>()->getInstrument(), testData.m_instrumentId2);
        ++it;
        ASSERT_NE(it, span.end());
        EXPECT_EQ(it->tryAs<bw_music::PercussionOffEvent>()->getInstrument(), testData.m_instrumentId2);
        ++it;
        EXPECT_EQ(it, span.end());
    }
}

// The set should be automatically selected based on the standard and instruments.
INSTANTIATE_TEST_SUITE_P(
    PercussionTest, SmfStandardPercussionTest,
    testing::Values(PercussionTestData{smf::GMSpecType::Value::GM, "AcBass", "HMTom", "OTrian"}, // GM Percussion
                    PercussionTestData{smf::GMSpecType::Value::GM2, "HighQ", "HMTom", "OSurdo"}, // Standard percussion
                    PercussionTestData{smf::GMSpecType::Value::GM2, "AcBass", "RLwTm2", "RHiTm1"}, // Room set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "PKick", "PLwTm2", "PHiTm1"},  // Power set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "ElBass", "ElSnr2", "RevCym"}, // Electronic set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "ARmSht", "Tamb", "AClavs"},   // Analog set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "JKick2", "HMTom", "OTrian"},  // Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "BrTap", "BrSlap", "BrSwrL"},  // Brush set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "TimpF", "Timpc", "Aplaus"},   // Orchestra set
                    PercussionTestData{smf::GMSpecType::Value::GM2, "GFret", "Bubble", "Dog"},     // SFX set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "FngSnp", "AcBass"},  // GS Standard 1 set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "RLwTm2", "RHiTm1"},  // GS Room set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "PLwTm2", "PHiTm1"},  // GS Power set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "ELwTm2", "RevCym"},  // GS Electronic set
                    PercussionTestData{smf::GMSpecType::Value::GS, "Bs909", "ELwTm2", "Bs808"},    // GS 808/909 set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "JKick1", "JKick2"},  // GS Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GS, "SnrRll", "JKick2", "BrTap"},   // GS Jazz set
                    PercussionTestData{smf::GMSpecType::Value::GS, "TimpF", "Timpc", "Aplaus"},    // GS Orchestra set
                    PercussionTestData{smf::GMSpecType::Value::GS, "Laugh", "Jetpln", "Aplaus"},   // GS SFX set
                    PercussionTestData{smf::GMSpecType::Value::XG, "SnrRll", "RimSht", "BeepLo"},  // XG Standard 1 set
                    PercussionTestData{smf::GMSpecType::Value::XG, "SnrRll", "RLwTm2", "BeepLo"},  // XG Room set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PKick", "PLwTm1", "BeepLo"},   // XG Rock set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PKick", "ELwTm1", "RevCym"},   // XG Electro set
                    PercussionTestData{smf::GMSpecType::Value::XG, "AnBass", "AMdTm2", "RevCym"},  // XG Analog set
                    PercussionTestData{smf::GMSpecType::Value::XG, "JKick1", "ClHHat", "OpHHat"},  // XG Jazz set
                    PercussionTestData{smf::GMSpecType::Value::XG, "JKick2", "BrTap", "BrSlap"},   // XG Brush set
                    PercussionTestData{smf::GMSpecType::Value::XG, "CnBD1", "CnCym2", "CnBD2"},    // XG Classic set
                    PercussionTestData{smf::GMSpecType::Value::XG, "PckScr", "Thnder", "XgMaou"},  // XG SFX1 set
                    PercussionTestData{smf::GMSpecType::Value::XG, "DoorCr", "Aplaus", "XgCstr"}   // XG SFX2 set
                    ));

namespace {
    struct TrackAllocationTestData {
        smf::GMSpecType::Value m_specificationId;
        std::vector<babelwires::ShortId> m_instrumentsInChannel[3];
        bool m_hasNotes[3];
        // The events expected to appear in channels 8, 9 and 10.
        std::vector<babelwires::ShortId> m_expectedInChannel[3];
    };
} // namespace

class SmfTrackAllocationPercussionTest : public testing::TestWithParam<TrackAllocationTestData> {};

TEST_P(SmfTrackAllocationPercussionTest, trackAllocation) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const TrackAllocationTestData& testData = GetParam();

    testUtils::TempFilePath tempFile("smfPercussionTrackAllocation.mid",
                                     smf::GMSpecType::getIdentifierFromValue(testData.m_specificationId).toString());

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.m_typeSystem, babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier());
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        smfType.getMeta().getSpec().set(testData.m_specificationId);
        auto tracks = smfType.getTrcks0();

        for (int i = 0; i < 3; ++i) {
            auto trackI = tracks.activateAndGetTrack(8 + i);

            bw_music::TrackBuilder track;

            track.addEvent(bw_music::NoteOnEvent{0, 65});
            track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 65});

            for (auto instrument : testData.m_instrumentsInChannel[i]) {
                track.addEvent(bw_music::PercussionOnEvent{0, instrument});
                track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 4), instrument});
            }

            trackI.set(track.finishAndGetTrack());
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
    }

    {
        babelwires::FileDataSource midiFile(tempFile);

        const auto feature = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_NE(feature, nullptr);

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
        ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

        auto tracks = smfSequence.getTrcks0();
        EXPECT_EQ(tracks->getNumChildren(), 3);

        const auto& metadata = smfSequence.getMeta();
        EXPECT_EQ(metadata.getSpec().get(), testData.m_specificationId);

        for (int i = 0; i < 3; ++i) {
            auto trackI = tracks.tryGetTrack(8 + i);
            ASSERT_TRUE(trackI);

            const auto& track = trackI->get();
            auto categoryMap = track.getNumEventGroupsByCategory();

            if (testData.m_hasNotes[i]) {
                ASSERT_NE(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
                EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory)->second, 1);
                EXPECT_EQ(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());
            } else {
                EXPECT_EQ(categoryMap.find(bw_music::NoteEvent::s_noteEventCategory), categoryMap.end());
                ASSERT_NE(categoryMap.find(bw_music::PercussionEvent::s_percussionEventCategory), categoryMap.end());

                std::vector<babelwires::ShortId> instrumentsInTrack;
                for (auto event : bw_music::iterateOver<bw_music::PercussionOnEvent>(track)) {
                    instrumentsInTrack.emplace_back(event.getInstrument());
                }

                EXPECT_TRUE(testUtils::areEqualSets(instrumentsInTrack, testData.m_expectedInChannel[i]));
            }
        }
    }
}

// Test how tracks get assigned in the various standards (the GS implementation in BabelWires-Music is not as flexible as the
// full standard)
INSTANTIATE_TEST_SUITE_P(
    PercussionTest, SmfTrackAllocationPercussionTest,
    testing::Values(TrackAllocationTestData{smf::GMSpecType::Value::GM,
                                            {{"AcBass", "HMTom", "OTrian"},
                                             {"AcBass", "HMTom", "OTrian"},
                                             {"AcBass", "HMTom", "OTrian"}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{
                        smf::GMSpecType::Value::GM2,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {true, false, false},
                        {{}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GM2,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{
                        smf::GMSpecType::Value::GS,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {true, false, false},
                        {{}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::GS,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {true, false, true},
                                            {{}, {"AcBass", "HMTom", "OTrian"}, {}}},
                    TrackAllocationTestData{
                        smf::GMSpecType::Value::XG,
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}},
                        {false, false, false},
                        {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}}},
                    TrackAllocationTestData{smf::GMSpecType::Value::XG,
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}},
                                            {false, false, true},
                                            {{"AcBass", "HMTom", "OTrian"}, {"AcBass", "HMTom", "OTrian"}, {}}}));