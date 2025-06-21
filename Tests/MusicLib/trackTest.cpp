#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testTrackEvents.hpp>


TEST(Track, Simple) {
    bw_music::UnsafeTrack track;
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

    bw_music::UnsafeTrack track;

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
        bw_music::UnsafeTrack track;

        for (int i = 0; i < 100; ++i) {
            track.addEvent(testUtils::TestTrackEventWithPayload(1, counter));
        }

        EXPECT_EQ(counter, 0);
    }

    EXPECT_EQ(counter, 100);
}

TEST(Track, equality) {
    bw_music::UnsafeTrack emptyTrack0;
    bw_music::UnsafeTrack emptyTrack1;
    bw_music::UnsafeTrack emptyTrackWithPositiveDuration;
    emptyTrackWithPositiveDuration.setDuration(1);
    
    bw_music::TrackBuilder trackWithNotesBuilder;
    bw_music::TrackBuilder trackWithSameNotesBuilder;
    bw_music::TrackBuilder trackWithMoreNotesBuilder;
    bw_music::TrackBuilder trackWithDifferentNotesBuilder;
    bw_music::TrackBuilder trackWithSameNotesLongerDurationBuilder;

    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithNotesBuilder);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotesBuilder);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65, 67}, trackWithMoreNotesBuilder);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 63, 65}, trackWithDifferentNotesBuilder);
    testUtils::addSimpleNotes(std::vector<bw_music::Pitch>{60, 62, 64, 65}, trackWithSameNotesLongerDurationBuilder);

    bw_music::Track trackWithNotes = trackWithNotesBuilder.finishAndGetTrack();
    bw_music::Track trackWithSameNotes = trackWithSameNotesBuilder.finishAndGetTrack();
    bw_music::Track trackWithMoreNotes = trackWithMoreNotesBuilder.finishAndGetTrack();
    bw_music::Track trackWithDifferentNotes = trackWithDifferentNotesBuilder.finishAndGetTrack();
    bw_music::Track trackWithSameNotesLongerDuration = trackWithSameNotesLongerDurationBuilder.finishAndGetTrack(16);

    EXPECT_EQ(emptyTrack0, emptyTrack0);
    EXPECT_EQ(emptyTrack0, emptyTrack1);
    EXPECT_NE(emptyTrack0, emptyTrackWithPositiveDuration);
    EXPECT_NE(emptyTrack0, trackWithNotes);
    EXPECT_EQ(trackWithNotes, trackWithSameNotes);
    EXPECT_NE(trackWithDifferentNotes, trackWithNotes);
    EXPECT_NE(trackWithNotes, trackWithMoreNotes);
    EXPECT_NE(trackWithNotes, trackWithSameNotesLongerDuration);
}