#include <gtest/gtest.h>

#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <MusicLib/Functions/monophonicSubtracksFunction.hpp>
#include <MusicLib/Processors/monophonicSubtracksProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    bw_music::Track getSamplePolyphonicTrack() {
        bw_music::TrackBuilder track;
        track.addEvent(bw_music::NoteOnEvent{0, 72});
        track.addEvent(bw_music::NoteOnEvent{0, 48});
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::NoteOffEvent{0, 48});
        track.addEvent(bw_music::NoteOnEvent{0, 74});
        track.addEvent(bw_music::NoteOnEvent{0, 50});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74});
        track.addEvent(bw_music::NoteOffEvent{0, 50});
        track.addEvent(bw_music::ChordOffEvent{0});
        track.addEvent(bw_music::NoteOnEvent{0, 76});
        track.addEvent(bw_music::NoteOnEvent{0, 52});
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 76});
        track.addEvent(bw_music::NoteOffEvent{0, 52});
        track.addEvent(bw_music::NoteOnEvent{0, 77});
        track.addEvent(bw_music::NoteOnEvent{0, 53});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 77});
        track.addEvent(bw_music::NoteOffEvent{0, 53});
        track.addEvent(bw_music::ChordOffEvent{0});
        return track.finishAndGetTrack();
    }

    bw_music::Track getStaggeredPolyphonicTrack() {
        bw_music::TrackBuilder track;
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}});
        track.addEvent(bw_music::NoteOnEvent{0, 48});
        track.addEvent(bw_music::NoteOnEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOnEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(bw_music::NoteOnEvent{0, 50});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOnEvent{0, 62});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::NoteOnEvent{0, 74});
        track.addEvent(bw_music::ChordOffEvent{0});
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 50});
        track.addEvent(bw_music::NoteOnEvent{0, 48});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 62});
        track.addEvent(bw_music::NoteOnEvent{0, 60});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74});
        track.addEvent(bw_music::NoteOnEvent{0, 72});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::ChordOffEvent{0});
        return track.finishAndGetTrack();
    }

    bw_music::Track getStaggeredPolyphonicTrack2() {
        bw_music::TrackBuilder track;
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}});
        track.addEvent(bw_music::NoteOnEvent{0, 72});
        track.addEvent(bw_music::NoteOnEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOnEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::NoteOnEvent{0, 74});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOnEvent{0, 62});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(bw_music::NoteOnEvent{0, 50});
        track.addEvent(bw_music::ChordOffEvent{0});
        track.addEvent(
            bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74});
        track.addEvent(bw_music::NoteOnEvent{0, 72});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 62});
        track.addEvent(bw_music::NoteOnEvent{0, 60});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 50});
        track.addEvent(bw_music::NoteOnEvent{0, 48});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60});
        track.addEvent(bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48});
        track.addEvent(bw_music::ChordOffEvent{0});
        return track.finishAndGetTrack();
    }
} // namespace

TEST(MonophonicSubtracksProcessorTest, simpleFunction) {
    bw_music::Track track = getSamplePolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 2, bw_music::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, FunctionLower) {
    bw_music::Track track = getSamplePolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 2, bw_music::MonophonicSubtracksPolicyEnum::Value::Low);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[1]);
    testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, redundantTracks) {
    bw_music::Track track = getSamplePolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 3, bw_music::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 3);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), 1);
    EXPECT_EQ(result.m_noteTracks[2].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);
    testUtils::testSimpleNotes({48, 50, 52, 53}, result.m_noteTracks[1]);
    EXPECT_EQ(result.m_noteTracks[2].getNumEvents(), 0);
    testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, eventToOther) {
    bw_music::Track track = getSamplePolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 1, bw_music::MonophonicSubtracksPolicyEnum::Value::High);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), 1);
    EXPECT_EQ(result.m_other.getDuration(), 1);

    testUtils::testSimpleNotes({72, 74, 76, 77}, result.m_noteTracks[0]);

    const std::vector<bw_music::TrackEventHolder> expectedEvents = {
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}},
        bw_music::NoteOnEvent{0, 48},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48},
        bw_music::NoteOnEvent{0, 50},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
        bw_music::NoteOffEvent{0, 50},
        bw_music::NoteOnEvent{0, 52},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 52},
        bw_music::NoteOnEvent{0, 53},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
        bw_music::NoteOffEvent{0, 53}};

    testUtils::testNotesAndChords(expectedEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, higherPitchesEvictOneTrack) {
    bw_music::Track track = getStaggeredPolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 1, bw_music::MonophonicSubtracksPolicyEnum::Value::HighEv);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<bw_music::TrackEventHolder> monoEvents = {
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(1, 4), 48},
        bw_music::NoteOnEvent{0, 60}, bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60},
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 72},
        bw_music::NoteOnEvent{0, 74}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 74},
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 72},
    };

    testUtils::testNotesAndChords(monoEvents, result.m_noteTracks[0]);

    const std::vector<bw_music::TrackEventHolder> otherEvents = {
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}},
        bw_music::NoteOnEvent{babelwires::Rational(3, 4), 50},
        bw_music::NoteOnEvent{babelwires::Rational(1, 4), 62},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 50},
        bw_music::NoteOnEvent{0, 48},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 62},
        bw_music::NoteOnEvent{0, 60},
        bw_music::NoteOffEvent{babelwires::Rational(1, 2), 48},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, higherPitchesEvictTwoTracks) {
    bw_music::Track track = getStaggeredPolyphonicTrack();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 2, bw_music::MonophonicSubtracksPolicyEnum::Value::HighEv);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<bw_music::TrackEventHolder> monoEvents0 = {
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(1, 2), 48},
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 72},
        bw_music::NoteOnEvent{0, 74}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 74},
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 72},
    };

    testUtils::testNotesAndChords(monoEvents0, result.m_noteTracks[0]);

    const std::vector<bw_music::TrackEventHolder> monoEvents1 = {
        bw_music::NoteOnEvent{babelwires::Rational(1, 4), 60},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 60},
        bw_music::NoteOnEvent{0, 62},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 62},
        bw_music::NoteOnEvent{0, 60},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 60}};

    testUtils::testNotesAndChords(monoEvents1, result.m_noteTracks[1]);

    const std::vector<bw_music::TrackEventHolder> otherEvents = {
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}},
        bw_music::NoteOnEvent{babelwires::Rational(3, 4), 50},
        bw_music::ChordOffEvent{babelwires::Rational(1, 2)},
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 50},
        bw_music::NoteOnEvent{0, 48},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 48},
        bw_music::ChordOffEvent{babelwires::Rational(1, 2)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, lowerPitchesEvictOneTrack) {
    bw_music::Track track = getStaggeredPolyphonicTrack2();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 1, bw_music::MonophonicSubtracksPolicyEnum::Value::LowEv);

    ASSERT_EQ(result.m_noteTracks.size(), 1);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<bw_music::TrackEventHolder> monoEvents = {
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(1, 4), 72},
        bw_music::NoteOnEvent{0, 60}, bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60},
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 48},
        bw_music::NoteOnEvent{0, 50}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 50},
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 48},
    };

    testUtils::testNotesAndChords(monoEvents, result.m_noteTracks[0]);

    const std::vector<bw_music::TrackEventHolder> otherEvents = {
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}},
        bw_music::NoteOnEvent{babelwires::Rational(3, 4), 74},
        bw_music::NoteOnEvent{babelwires::Rational(1, 4), 62},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74},
        bw_music::NoteOnEvent{0, 72},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 62},
        bw_music::NoteOnEvent{0, 60},
        bw_music::NoteOffEvent{babelwires::Rational(1, 2), 72},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 60},
        bw_music::ChordOffEvent{babelwires::Rational(1, 4)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, lowerPitchesEvictTwoTracks) {
    bw_music::Track track = getStaggeredPolyphonicTrack2();

    bw_music::MonophonicSubtracksResult result =
        bw_music::getMonophonicSubtracks(track, 2, bw_music::MonophonicSubtracksPolicyEnum::Value::LowEv);

    ASSERT_EQ(result.m_noteTracks.size(), 2);
    EXPECT_EQ(result.m_noteTracks[0].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_noteTracks[1].getDuration(), babelwires::Rational(11, 4));
    EXPECT_EQ(result.m_other.getDuration(), babelwires::Rational(11, 4));

    const std::vector<bw_music::TrackEventHolder> monoEvents0 = {
        bw_music::NoteOnEvent{0, 72}, bw_music::NoteOffEvent{babelwires::Rational(1, 2), 72},
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 48},
        bw_music::NoteOnEvent{0, 50}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 50},
        bw_music::NoteOnEvent{0, 48}, bw_music::NoteOffEvent{babelwires::Rational(3, 4), 48},
    };

    testUtils::testNotesAndChords(monoEvents0, result.m_noteTracks[0]);

    const std::vector<bw_music::TrackEventHolder> monoEvents1 = {
        bw_music::NoteOnEvent{babelwires::Rational(1, 4), 60},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 60},
        bw_music::NoteOnEvent{0, 62},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 62},
        bw_music::NoteOnEvent{0, 60},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 60}};

    testUtils::testNotesAndChords(monoEvents1, result.m_noteTracks[1]);

    const std::vector<bw_music::TrackEventHolder> otherEvents = {
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M}},
        bw_music::NoteOnEvent{babelwires::Rational(3, 4), 74},
        bw_music::ChordOffEvent{babelwires::Rational(1, 2)},
        bw_music::ChordOnEvent{0, {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
        bw_music::NoteOffEvent{babelwires::Rational(1, 4), 74},
        bw_music::NoteOnEvent{0, 72},
        bw_music::NoteOffEvent{babelwires::Rational(3, 4), 72},
        bw_music::ChordOffEvent{babelwires::Rational(1, 2)},
    };

    testUtils::testNotesAndChords(otherEvents, result.m_other);
}

TEST(MonophonicSubtracksProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::MonophonicSubtracksProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    auto input = bw_music::MonophonicSubtracksProcessorInput::Instance(processor.getInput());
    const auto output = bw_music::MonophonicSubtracksProcessorOutput::ConstInstance(processor.getOutput());

    input.getNumTrk().set(2);
    input.getPolicy().set(bw_music::MonophonicSubtracksPolicyEnum::Value::Low);
    input.getInput().set(getSamplePolyphonicTrack());

    processor.process(testEnvironment.m_log);

    EXPECT_EQ(output.getSbtrks().getSize(), 2);
    testUtils::testSimpleNotes({48, 50, 52, 53}, output.getSbtrks().getEntry(0).get());
    testUtils::testSimpleNotes({72, 74, 76, 77}, output.getSbtrks().getEntry(1).get());
    testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          output.getOther().get());

    processor.getInput().clearChanges();
    {
        input.getNumTrk().set(3);
        input.getPolicy().set(bw_music::MonophonicSubtracksPolicyEnum::Value::High);
    }
    processor.process(testEnvironment.m_log);

    EXPECT_EQ(output.getSbtrks().getSize(), 3);
    testUtils::testSimpleNotes({72, 74, 76, 77}, output.getSbtrks().getEntry(0).get());
    testUtils::testSimpleNotes({48, 50, 52, 53}, output.getSbtrks().getEntry(1).get());
    EXPECT_EQ(output.getSbtrks().getEntry(2).get().getNumEvents(), 0);
    testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                           {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m}},
                          output.getOther().get());
}