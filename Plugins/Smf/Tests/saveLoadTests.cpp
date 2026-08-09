#include <gtest/gtest.h>

#include <Smf/libRegistration.hpp>
#include <Smf/midiTrackAndChannel.hpp>
#include <Smf/midiTrackAndChannelArray.hpp>
#include <Smf/smfParser.hpp>
#include <Smf/smfWriter.hpp>

#include <MusicLib/Types/Track/TrackEvents/notePressureEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/panEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/pitchBendEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/pressureEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/tempoEvent.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/Instance/arrayTypeInstance.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>
#include <BabelWiresLib/libRegistration.hpp>

#include <BaseLib/IO/fileDataSource.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

#include <Tests/TestUtils/tempFilePath.hpp>

using bw_music::operator""_mmv16;

TEST(SmfSaveLoadTest, cMajorScale) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    testUtils::TempFilePath tempFile("cMajor.mid");

    const std::vector<bw_music::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};
    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
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
            metadata.activateAndGetName().set(u8"Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            metadata.activateAndGetCopyR().set(u8"(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            bw_music::TrackBuilder globalTrack;
            globalTrack.addEvent(bw_music::TempoEvent(0, 100));
            smfType.getGlobal().set(globalTrack.finishAndGetTrack());
        }
    }

    void checkMetadata(const smf::SmfSequence::ConstInstance& smfType, std::uint8_t flags) {
        const auto& metadata = smfType.getMeta();

        if (flags & HAS_SEQUENCE_NAME) {
            ASSERT_TRUE(metadata.tryGetName());
            EXPECT_EQ(metadata.tryGetName()->get(), u8"Test Sequence Name");
        }
        if (flags & HAS_COPYRIGHT) {
            ASSERT_TRUE(metadata.tryGetCopyR());
            EXPECT_EQ(metadata.tryGetCopyR()->get(), u8"(C)2021 Test Copyright");
        }
        if (flags & HAS_TEMPO) {
            ASSERT_TRUE(metadata.tryGetITempo());
            EXPECT_EQ(metadata.tryGetITempo()->get(), 100);

            const auto& globalTrack = smfType.getGlobal().get();
            auto [tempoBegin, tempoEnd] = bw_music::iterateOver<bw_music::TempoEvent>(globalTrack);
            ASSERT_NE(tempoBegin, tempoEnd);
            EXPECT_EQ(tempoBegin->getBpm(), 100);
            ++tempoBegin;
            EXPECT_EQ(tempoBegin, tempoEnd);
        }
    }
} // namespace

namespace {
    bw_music::Track makeChannelVoiceCoverageTrack() {
        bw_music::TrackBuilder track;
        track.addEvent(bw_music::NoteOnEvent(0, 60, bw_music::MinMaxValue16::assertFromUnsigned<7>(100u)));
        track.addEvent(bw_music::NotePressureEvent(babelwires::Rational(1, 16), 60, bw_music::ControllerStorage::assertFromUnsigned<7>(96u)));
        track.addEvent(bw_music::PanEvent(babelwires::Rational(1, 16),
                                          bw_music::CentredControllerStorage::assertFromUnsigned<32>(0u)));
        track.addEvent(bw_music::VolumeEvent(babelwires::Rational(1, 16),
                                             bw_music::ControllerStorage::assertFromUnsigned<7>(127u)));
        track.addEvent(bw_music::ExpressionEvent(babelwires::Rational(1, 16),
                                                 bw_music::ControllerStorage::assertFromUnsigned<7>(0u)));
        track.addEvent(bw_music::SustainEvent(babelwires::Rational(1, 16),
                                              bw_music::ControllerStorage::assertFromUnsigned<7>(127u)));
        track.addEvent(bw_music::PitchBendEvent(babelwires::Rational(1, 16),
                                                bw_music::CentredControllerStorage::assertFromUnsigned<14>(0u)));
        track.addEvent(bw_music::PressureEvent(babelwires::Rational(1, 16),
                                               bw_music::ControllerStorage::assertFromUnsigned<7>(127u)));
        track.addEvent(bw_music::SustainEvent(babelwires::Rational(1, 16),
                                              bw_music::ControllerStorage::assertFromUnsigned<7>(0u)));
        track.addEvent(bw_music::PitchBendEvent(babelwires::Rational(1, 16),
                                                bw_music::CentredControllerStorage::assertFromUnsigned<14>(0x3fffu)));
        track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 16), 60,
                                              bw_music::MinMaxValue16::assertFromUnsigned<7>(60u)));
        return track.finishAndGetTrack();
    }
} // namespace

TEST(SmfSaveLoadTest, cMajorScaleWithMetadata) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));

    const std::vector<bw_music::Pitch> pitches{60, 62, 64, 65, 67, 69, 71, 72};

    for (std::uint8_t metadata = 0; metadata < 8; ++metadata) {
        testUtils::TempFilePath tempFile("cMajorWithMetadata.mid", metadata);
        {
            babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                                 babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                     testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));
    testUtils::TempFilePath tempFile("format0Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
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
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));
    testUtils::TempFilePath tempFile("format1Chords.mid");

    const char* trackName[3] = {"ch0", "ch1", "ch2"};

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
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

TEST(SmfSaveLoadTest, format1TempoGlobalTrack) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));
    testUtils::TempFilePath tempFile("format1TempoGlobalTrack.mid");

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        smfType.selectTag("SMF1");

        bw_music::TrackBuilder globalTrack;
        globalTrack.addEvent(bw_music::TempoEvent(0, 100));
        smfType.getGlobal().set(globalTrack.finishAndGetTrack());

        auto tracks = smfType.getTrcks1();
        tracks.setSize(1);
        auto trackAndChan = tracks.getEntry(0);
        trackAndChan.getChan().set(0);

        bw_music::TrackBuilder track;
        testUtils::addSimpleNotes(chordPitches[0], track);
        trackAndChan.getTrack().set(track.finishAndGetTrack());

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
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

    auto tracks = smfSequence.getTrcks1();
    EXPECT_EQ(tracks.getSize(), 1);
    testUtils::testSimpleNotes(chordPitches[0], tracks.getEntry(0).getTrack().get());

    const auto& globalTrack = smfSequence.getGlobal().get();
    auto [tempoBegin, tempoEnd] = bw_music::iterateOver<bw_music::TempoEvent>(globalTrack);
    ASSERT_NE(tempoBegin, tempoEnd);
    EXPECT_EQ(tempoBegin->getBpm(), 100);
    ++tempoBegin;
    EXPECT_EQ(tempoBegin, tempoEnd);
}

TEST(SmfSaveLoadTest, format0ChannelVoiceEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));
    testUtils::TempFilePath tempFile("format0ChannelVoiceEvents.mid");

    const bw_music::Track expectedTrack = makeChannelVoiceCoverageTrack();

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        smfType.getTrcks0().activateAndGetTrack(3).set(makeChannelVoiceCoverageTrack());

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

    const auto track = smfSequence.getTrcks0().tryGetTrack(3);
    ASSERT_TRUE(track);
    EXPECT_TRUE(track->get() == expectedTrack);
}

TEST(SmfSaveLoadTest, format1ChannelVoiceEvents) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);
    ASSERT_TRUE(smf::registerLib(testEnvironment.m_projectContext, testEnvironment.m_log));
    testUtils::TempFilePath tempFile("format1ChannelVoiceEvents.mid");

    const bw_music::Track expectedTrack = makeChannelVoiceCoverageTrack();

    {
        babelwires::ValueTreeRoot smfFeature(testEnvironment.m_projectContext.get<babelwires::TypeSystem>(),
                                             babelwires::FileTypeT<smf::SmfSequence>::getType(
                                                 testEnvironment.m_projectContext.get<babelwires::TypeSystem>()));
        smfFeature.setToDefault();

        smf::SmfSequence::Instance smfType{smfFeature.getChild(0)->as<babelwires::ValueTreeNode>()};
        smfType.selectTag("SMF1");

        auto tracks = smfType.getTrcks1();
        tracks.setSize(1);
        auto trackAndChan = tracks.getEntry(0);
        trackAndChan.getChan().set(5);
        trackAndChan.getTrack().set(makeChannelVoiceCoverageTrack());

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
    ASSERT_EQ(smfSequence.getInstanceType().getIndexOfTag(smfSequence.getSelectedTag()), 1);

    auto tracks = smfSequence.getTrcks1();
    ASSERT_EQ(tracks.getSize(), 1);
    const auto track = tracks.getEntry(0);
    EXPECT_EQ(track.getChan().get(), 5);
    EXPECT_TRUE(track.getTrack().get() == expectedTrack);
}
