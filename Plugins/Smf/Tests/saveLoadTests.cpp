#include <gtest/gtest.h>

#include <Plugins/Smf/Plugin/libRegistration.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>
#include <Plugins/Smf/Plugin/smfParser.hpp>
#include <Plugins/Smf/Plugin/smfWriter.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/Instance/arrayTypeInstance.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>
#include <BabelWiresLib/libRegistration.hpp>

#include <BaseLib/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/tempFilePath.hpp>

TEST(SmfSaveLoadTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    testUtils::TempFilePath tempFile("cMajor.mid");

    const std::vector<bw_music::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};
    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.m_typeSystem,
                                             babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier().assertResolve(
                                                 testEnvironment.m_projectContext.m_typeSystem));
        smfFeature.setToDefault();

        babelwires::FileTypeT<smf::SmfSequence>::Instance smfSequence{smfFeature};
        smf::SmfSequence::Instance smfType = smfSequence.getConts();
        auto tracks = smfType.getTrcks0();
        auto track2 = tracks.activateAndGetTrack(2);

        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes(pitches, track);
        track2.set(track.finishAndGetTrack());

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
    }

    {
        auto midiFileResult = babelwires::FileDataSource::open(tempFile);
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

        auto track2 = tracks.tryGetTrack(2);
        ASSERT_TRUE(track2);

        testUtils::testSimpleNotes(pitches, track2->get());
    }
}

namespace {

    enum MetadataFlags { HAS_SEQUENCE_NAME = 0b001, HAS_COPYRIGHT = 0b010, HAS_TEMPO = 0b100 };

    void addMetadata(smf::SmfSequence::Instance& smfType, std::uint8_t flags) {
        auto metadata = smfType.getMeta();

        if (flags & HAS_SEQUENCE_NAME) {
            metadata.activateAndGetName().set("Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            metadata.activateAndGetCopyR().set("(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            metadata.activateAndGetTempo().set(100);
        }
    }

    void checkMetadata(const smf::SmfSequence::ConstInstance& smfType, std::uint8_t flags) {
        const auto& metadata = smfType.getMeta();

        if (flags & HAS_SEQUENCE_NAME) {
            ASSERT_TRUE(metadata.tryGetName());
            EXPECT_EQ(metadata.tryGetName()->get(), "Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            ASSERT_TRUE(metadata.tryGetCopyR());
            EXPECT_EQ(metadata.tryGetCopyR()->get(), "(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            ASSERT_TRUE(metadata.tryGetTempo());
            EXPECT_EQ(metadata.tryGetTempo()->get(), 100);
        }
    }
} // namespace

TEST(SmfSaveLoadTest, cMajorScaleWithMetadata) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);

    const std::vector<bw_music::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};

    for (std::uint8_t metadata = 0; metadata < 8; ++metadata) {
        testUtils::TempFilePath tempFile("cMajorWithMetadata.mid", metadata);
        {
            babelwires::ValueTreeRoot smfFeature(
                testEnvironment.m_projectContext.m_typeSystem,
                babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier().assertResolve(
                    testEnvironment.m_projectContext.m_typeSystem));
            smfFeature.setToDefault();

            smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};

            addMetadata(smfType, metadata);

            auto tracks = smfType.getTrcks0();
            auto track2 = tracks.activateAndGetTrack(2);

            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(pitches, track);
            track2.set(track.finishAndGetTrack());

            std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
            smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
        }

        auto midiFileResult = babelwires::FileDataSource::open(tempFile);
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);

        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_TRUE(midiFile.close().has_value());
        ASSERT_TRUE(result.has_value());
        const auto& feature = *result;

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
        ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

        checkMetadata(smfSequence, metadata);

        auto tracks = smfSequence.getTrcks0();
        EXPECT_EQ(tracks->getNumChildren(), 1);

        auto track2 = tracks.tryGetTrack(2);
        ASSERT_TRUE(track2);

        testUtils::testSimpleNotes(pitches, track2->get());
    }
}

namespace {
    const std::vector<bw_music::Pitch> chordPitches[3] = {
        {60, 62, 64, 65, 67, 69, 71, 72}, {64, 65, 67, 69, 71, 72, 74, 76}, {67, 69, 71, 72, 74, 76, 77, 79}};
}

TEST(SmfSaveLoadTest, format0Chords) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);
    testUtils::TempFilePath tempFile("format0Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.m_typeSystem,
                                             babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier().assertResolve(
                                                 testEnvironment.m_projectContext.m_typeSystem));
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        auto tracks = smfType.getTrcks0();

        for (int i = 0; i < 3; ++i) {
            auto trackI = tracks.activateAndGetTrack(i);
            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(chordPitches[i], track);
            trackI.set(track.finishAndGetTrack());
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
    }

    {
        auto midiFileResult = babelwires::FileDataSource::open(tempFile);
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);

        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_TRUE(midiFile.close().has_value());
        ASSERT_TRUE(result.has_value());
        const auto& feature = *result;

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
        ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 0);

        auto tracks = smfSequence.getTrcks0();
        EXPECT_EQ(tracks->getNumChildren(), 3);

        for (int i = 0; i < 3; ++i) {
            auto trackI = tracks.tryGetTrack(i);
            ASSERT_TRUE(trackI);
            testUtils::testSimpleNotes(chordPitches[i], trackI->get());
        }
    }
}

TEST(SmfSaveLoadTest, format1Chords) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    smf::registerLib(testEnvironment.m_projectContext);
    testUtils::TempFilePath tempFile("format1Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.m_typeSystem,
                                             babelwires::FileTypeT<smf::SmfSequence>::getThisIdentifier().assertResolve(
                                                 testEnvironment.m_projectContext.m_typeSystem));
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};

        smfType.selectTag("SMF1");
        auto tracks = smfType.getTrcks1();
        tracks.setSize(3);

        for (int i = 0; i < 3; ++i) {
            auto trackAndChan = tracks.getEntry(i);
            trackAndChan.getChan().set(i);
            bw_music::TrackBuilder track;
            testUtils::addSimpleNotes(chordPitches[i], track);
            trackAndChan.getTrack().set(track.finishAndGetTrack());
        }

        std::ofstream os = tempFile.openForWriting(std::ios_base::binary);
        smf::writeToSmf(testEnvironment.m_projectContext, testEnvironment.m_log, smfFeature, os);
    }

    {
        auto midiFileResult = babelwires::FileDataSource::open(tempFile);
        ASSERT_TRUE(midiFileResult.has_value());
        auto midiFile = std::move(*midiFileResult);

        auto result = smf::parseSmfSequence(midiFile, testEnvironment.m_projectContext, testEnvironment.m_log);
        ASSERT_TRUE(midiFile.close().has_value());
        ASSERT_TRUE(result.has_value());
        const auto& feature = *result;

        smf::SmfSequence::ConstInstance smfSequence{feature->getChild(0)->as<babelwires::ValueTreeNode>()};
        ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

        auto tracks = smfSequence.getTrcks1();
        EXPECT_EQ(tracks.getSize(), 3);

        for (int i = 0; i < 3; ++i) {
            auto track = tracks.getEntry(i);
            EXPECT_EQ(track.getChan().get(), i);
            testUtils::testSimpleNotes(chordPitches[i], track.getTrack().get());
        }
    }
}
