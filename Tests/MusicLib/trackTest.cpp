#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testTrackEvents.hpp>


TEST(Track, Simple) {
    bw_music::Track track;
    const size_t hashWhenEmpty = track.getHash();

    EXPECT_EQ(track.getNumEvents(), 0);
    EXPECT_EQ(track.getDuration(), 0);

    testUtils::TestTrackEvent event0(0);
    track.addEvent(event0);

    EXPECT_EQ(track.getNumEvents(), 1);
    EXPECT_EQ(track.getDuration(), 0);
    const size_t hashWithOneEvent = track.getHash();
    EXPECT_NE(hashWhenEmpty, hashWithOneEvent);

    testUtils::TestTrackEvent event1(0);
    event1.setTimeSinceLastEvent(1);
    track.addEvent(event1);

    EXPECT_EQ(track.getNumEvents(), 2);
    EXPECT_EQ(track.getDuration(), 1);
    const size_t hashWithTwoEvents = track.getHash();
    EXPECT_NE(hashWhenEmpty, hashWithTwoEvents);
    EXPECT_NE(hashWithOneEvent, hashWithTwoEvents);

    int count = 0;
    for (const auto& event : static_cast<const bw_music::Track&>(track)) {
        ++count;
    }
    EXPECT_EQ(count, 2);
}

TEST(Track, BlocksAndAlignment) {
    EXPECT_NE(testUtils::TestTrackEvent(1).getSize(), testUtils::BigTestTrackEvent(1).getSize());
    EXPECT_NE(testUtils::TestTrackEvent(1).getAlignment(), testUtils::BigTestTrackEvent(1).getAlignment());

    bw_music::Track track;

    for (int i = 0; i < 100; ++i) {
        testUtils::TestTrackEvent newEvent(1);
        track.addEvent(std::move(newEvent));
        testUtils::BigTestTrackEvent newEvent2(1);
        newEvent2.m_big[2] = i;
        track.addEvent(std::move(newEvent2));
    }
    EXPECT_EQ(track.getNumEvents(), 200);
    EXPECT_EQ(track.getDuration(), 200);

    int count = 0;
    for (const auto& event : track) {
        EXPECT_EQ(event.getTimeSinceLastEvent(), 1);
        if (count % 2) {
            EXPECT_NE(event.as<testUtils::BigTestTrackEvent>(), nullptr);
            const testUtils::BigTestTrackEvent& e = static_cast<const testUtils::BigTestTrackEvent&>(event);
            EXPECT_EQ(e.m_big[2], (count / 2));
        }
        ++count;
    }
    EXPECT_EQ(count, 200);
}

TEST(Track, PayloadTest) {
    int counter = 0;

    {
        bw_music::Track track;

        for (int i = 0; i < 100; ++i) {
            track.addEvent(testUtils::TestTrackEventWithPayload(1, counter));
        }

        EXPECT_EQ(counter, 0);
    }

    EXPECT_EQ(counter, 100);
}

TEST(Track, equality) {
    bw_music::Track emptyTrack0;
    bw_music::Track emptyTrack1;
    bw_music::Track emptyTrackWithPositiveDuration;
    bw_music::Track trackWithNotes;
    bw_music::Track trackWithSameNotes;
    bw_music::Track trackWithMoreNotes;
    bw_music::Track trackWithDifferentNotes;
    bw_music::Track trackWithSameNotesLongerDuration;

    emptyTrackWithPositiveDuration.setDuration(1);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67}, trackWithMoreNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 63, 65}, trackWithDifferentNotes);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotesLongerDuration);
    trackWithSameNotesLongerDuration.setDuration(16);

    EXPECT_EQ(emptyTrack0, emptyTrack0);
    EXPECT_EQ(emptyTrack0, emptyTrack1);
    EXPECT_NE(emptyTrack0, emptyTrackWithPositiveDuration);
    EXPECT_NE(emptyTrack0, trackWithNotes);
    EXPECT_EQ(trackWithNotes, trackWithSameNotes);
    EXPECT_NE(trackWithDifferentNotes, trackWithNotes);
    EXPECT_NE(trackWithNotes, trackWithMoreNotes);
    EXPECT_NE(trackWithNotes, trackWithSameNotesLongerDuration);
}