#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Utilities/trackBuilder.hpp>
#include <MusicLib/Utilities/simpleTrackBuilder.hpp>
#include <MusicLib/Utilities/trackValidator.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

#include <array>

namespace {
    // Note that events belong to note groups do not have to derive from NoteEvent.
    struct TestEnclosedEvent : bw_music::TrackEvent {
        STREAM_EVENT(TestEnclosedEvent);
        
        TestEnclosedEvent(bw_music::ModelDuration timeSinceLastEvent, bw_music::Pitch pitch)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch) {}

        bw_music::TrackEvent::GroupingInfo getGroupingInfo() const override {
            return {bw_music::NoteEvent::s_noteEventCategory, m_pitch,
                    bw_music::TrackEvent::GroupingInfo::Grouping::EnclosedInGroup};
        }

        bw_music::Pitch m_pitch;
    };
} // namespace

TEST(TrackBuilderTest, validator_validSimple) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_TRUE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, validator_invalidSimpleZeroLengthNote) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(0, 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, validator_invalidSimpleEndEventOutsideGroup) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, validator_invalidStartEventInsideGroup) {
    bw_music::Track track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, simpleBuilder_validSimple) {
    bw_music::Track track;
    bw_music::SimpleTrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_TRUE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_validSimple) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_TRUE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidSimpleZeroLengthNote) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(0, 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidSimpleEndEventOutsideGroup) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidStartEventInsideGroup) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidReordered) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOffEvent(0, 60));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOffEvent(0, 60));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOffEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidReorderedAndZeroLength) {
    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents() - 2);
}

TEST(TrackBuilderTest, validBuilder_InvalidMixture) {
    bw_music::Track goodEvents;
    goodEvents.addEvent(bw_music::NoteOnEvent(0, 60));
    goodEvents.addEvent(TestEnclosedEvent(bw_music::ModelDuration(1, 8), 60));
    goodEvents.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 8), 60));
    goodEvents.addEvent(bw_music::NoteOnEvent(0, 62));
    goodEvents.addEvent(TestEnclosedEvent(bw_music::ModelDuration(1, 8), 62));
    goodEvents.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 8), 62));
    goodEvents.addEvent(bw_music::NoteOnEvent(0, 64));
    goodEvents.addEvent(TestEnclosedEvent(bw_music::ModelDuration(1, 8), 64));
    goodEvents.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 8), 64));
    goodEvents.addEvent(bw_music::NoteOnEvent(0, 67));
    goodEvents.addEvent(TestEnclosedEvent(bw_music::ModelDuration(1, 8), 67));
    goodEvents.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 8), 67));

    bw_music::Track badEvents;
    badEvents.addEvent(TestEnclosedEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(TestEnclosedEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOnEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOnEvent(0, 72));

    bw_music::Track track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    // getBadEvent() repeatedly produces badEvents
    auto badEventIt = badEvents.end();
    auto getBadEvent = [&badEventIt, &badEvents] () -> const bw_music::TrackEvent& {
        if (badEventIt == badEvents.end()) {
            badEventIt = badEvents.begin();
        }
        return *badEventIt++;
    };

    for (const auto& goodEvent : goodEvents) {
        bw_music::TrackEventHolder g = goodEvent;
        bw_music::TrackEventHolder b = getBadEvent();
        b->setTimeSinceLastEvent(g->getTimeSinceLastEvent());
        g->setTimeSinceLastEvent(0);
        addEvent(b.release());
        addEvent(g.release());
        addEvent(getBadEvent());
        addEvent(getBadEvent());
    }

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(builtTrack.getTotalEventDuration(), goodEvents.getTotalEventDuration());

    unsigned int goodEventCount = 0;
    for (const auto& builtEvent : builtTrack) {
        if (builtEvent.getGroupingInfo().m_groupValue < 72) {
            ++goodEventCount;
        }
    }
    EXPECT_EQ(goodEventCount, goodEvents.getNumEvents());
}
