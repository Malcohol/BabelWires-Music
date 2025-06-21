#include <gtest/gtest.h>

#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>
#include <MusicLib/Utilities/trackTraverser.hpp>

#include <Tests/TestUtils/testTrackEvents.hpp>

TEST(TrackTraverser, leastUpperBoundDuration) {
    bw_music::Track track;

    track.setDuration(10);

    bw_music::TrackTraverser traverser(track, track);

    bw_music::ModelDuration trackDuration = 0;
    traverser.leastUpperBoundDuration(trackDuration);
    EXPECT_EQ(trackDuration, 10);

    track.setDuration(20);
    traverser.leastUpperBoundDuration(trackDuration);
    EXPECT_EQ(trackDuration, 20);

    track.setDuration(5);
    traverser.leastUpperBoundDuration(trackDuration);
    EXPECT_EQ(trackDuration, 20);
}

TEST(TrackTraverser, greatestLowerBoundNextEvent) {
    bw_music::TrackBuilder track1Builder;
    bw_music::TrackBuilder track2Builder;
    bw_music::TrackBuilder track3Builder;
    track1Builder.addEvent(testUtils::TestTrackEvent(10, 0));
    track2Builder.addEvent(testUtils::TestTrackEvent(5, 0));
    track3Builder.addEvent(testUtils::TestTrackEvent2(20, 0));
    bw_music::Track track1 = track1Builder.finishAndGetTrack();
    bw_music::Track track2 = track2Builder.finishAndGetTrack();
    bw_music::Track track3 = track3Builder.finishAndGetTrack();

    bw_music::TrackTraverser traverser1(track1, track1);
    bw_music::TrackTraverser traverser2(track2, track2);
    bw_music::TrackTraverser traverser3(track3, track3);

    bw_music::ModelDuration timeToNextEvent = std::numeric_limits<bw_music::ModelDuration>::max();
    traverser1.greatestLowerBoundNextEvent(timeToNextEvent);
    EXPECT_EQ(timeToNextEvent, 10);

    traverser2.greatestLowerBoundNextEvent(timeToNextEvent);
    EXPECT_EQ(timeToNextEvent, 5);

    traverser3.greatestLowerBoundNextEvent(timeToNextEvent);
    EXPECT_EQ(timeToNextEvent, 5);
}

TEST(TrackTraverser, filteredIteration) {
    bw_music::TrackBuilder trackBuilder;

    for (int i = 0; i < 10; ++i) {
        trackBuilder.addEvent(testUtils::TestTrackEvent(1, 2 * i));
        trackBuilder.addEvent(testUtils::TestTrackEvent(0, (2 * i) + 1));
        trackBuilder.addEvent(testUtils::TestTrackEvent2(0, 2 * i));
        trackBuilder.addEvent(testUtils::TestTrackEvent2(1, (2 * i) + 1));
    }
    bw_music::Track track = trackBuilder.finishAndGetTrack();

    bw_music::TrackTraverser<bw_music::FilteredTrackIterator<testUtils::TestTrackEvent>> traverser1(
        track, bw_music::iterateOver<testUtils::TestTrackEvent>(track));
    bw_music::TrackTraverser<bw_music::FilteredTrackIterator<testUtils::TestTrackEvent2>> traverser2(
        track, bw_music::iterateOver<testUtils::TestTrackEvent2>(track));

    bw_music::ModelDuration trackDuration = 0;
    traverser1.leastUpperBoundDuration(trackDuration);
    EXPECT_EQ(trackDuration, track.getDuration());
    traverser2.leastUpperBoundDuration(trackDuration);
    EXPECT_EQ(trackDuration, track.getDuration());

    bw_music::ModelDuration totalEventDuration = 0;
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(traverser1.hasMoreEvents());
        EXPECT_TRUE(traverser2.hasMoreEvents());
        bw_music::ModelDuration timeToNextEvent = std::numeric_limits<bw_music::ModelDuration>::max();
        traverser1.greatestLowerBoundNextEvent(timeToNextEvent);
        traverser2.greatestLowerBoundNextEvent(timeToNextEvent);
        EXPECT_EQ(timeToNextEvent, 1);
        totalEventDuration += timeToNextEvent;
        {
            decltype(traverser1)::EventsAtTime events1;
            traverser1.advance(timeToNextEvent, events1);
            ASSERT_EQ(events1.size(), 2);
            const auto& event1_0 = events1[0];
            EXPECT_EQ(event1_0->m_value, 2 * i);
            const auto& event1_1 = events1[1];
            EXPECT_EQ(event1_1->m_value, (2 * i) + 1);
        }
        {
            decltype(traverser2)::EventsAtTime events2;
            traverser2.advance(timeToNextEvent, events2);
            ASSERT_EQ(events2.size(), 1);
            const auto& event2_0 = events2[0];
            EXPECT_EQ(event2_0->m_value, 2 * i);
        }
        timeToNextEvent = std::numeric_limits<bw_music::ModelDuration>::max();
        traverser1.greatestLowerBoundNextEvent(timeToNextEvent);
        traverser2.greatestLowerBoundNextEvent(timeToNextEvent);
        EXPECT_EQ(timeToNextEvent, 1);
        totalEventDuration += timeToNextEvent;
        {
            decltype(traverser1)::EventsAtTime events1;
            traverser1.advance(timeToNextEvent, events1);
            EXPECT_TRUE(events1.empty());
        }
        {
            decltype(traverser2)::EventsAtTime events2;
            traverser2.advance(timeToNextEvent, events2);
            ASSERT_EQ(events2.size(), 1);
            const auto& event2_0 = events2[0];
            EXPECT_EQ(event2_0->m_value, (2 * i) + 1);
        }
    }
    EXPECT_FALSE(traverser1.hasMoreEvents());
    EXPECT_FALSE(traverser2.hasMoreEvents());
    EXPECT_EQ(totalEventDuration, track.getDuration());
}
