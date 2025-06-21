#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/trackValidator.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>

#include <array>

namespace {
    struct TestEnclosedEvent : bw_music::TrackEvent {
        STREAM_EVENT(TestEnclosedEvent);

        TestEnclosedEvent(bw_music::ModelDuration timeSinceLastEvent, bw_music::Pitch pitch)
            : TrackEvent(timeSinceLastEvent)
            , m_pitch(pitch) {}

        bw_music::TrackEvent::GroupingInfo getGroupingInfo() const override {
            return {bw_music::NoteEvent::s_noteEventCategory, m_pitch,
                    bw_music::TrackEvent::GroupingInfo::Grouping::EnclosedInGroup};
        }
        void createEndEvent(bw_music::TrackEventHolder& dest,
                            bw_music::ModelDuration timeSinceLastEvent) const override {}
        bw_music::Pitch m_pitch;
    };
} // namespace

TEST(TrackBuilderTest, validator_validSimple) {
    bw_music::UnsafeTrack track;

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
    bw_music::UnsafeTrack track;

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
    bw_music::UnsafeTrack track;

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
    bw_music::UnsafeTrack track;

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

TEST(TrackBuilderTest, validator_invalidUnterminatedGroup) {
    bw_music::UnsafeTrack track;

    track.addEvent(bw_music::NoteOnEvent(0, 60));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    track.addEvent(bw_music::NoteOnEvent(0, 62));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    track.addEvent(bw_music::NoteOnEvent(0, 64));
    track.addEvent(bw_music::NoteOnEvent(0, 72)); // unterminated
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    track.addEvent(bw_music::NoteOnEvent(0, 67));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));

    EXPECT_FALSE(bw_music::isTrackValid(track));
}

TEST(TrackBuilderTest, simpleBuilder_validSimple) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_validSimple) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidSimpleZeroLengthNote) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidSimpleEndEventOutsideGroup) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidStartEventInsideGroup) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_GT(track.getNumEvents(), builtTrack.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidReordered) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_invalidUnterminatedGroup) {
    bw_music::UnsafeTrack track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 72)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 84)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));
    addEvent(bw_music::NoteOnEvent(0, 48)); // unterminated and at the end of the track

    EXPECT_FALSE(bw_music::isTrackValid(track));

    auto builtTrack = trackBuilder.finishAndGetTrack();
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    // Two events added and one event removed
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents() + 1);
}

TEST(TrackBuilderTest, validBuilder_invalidUnterminatedGroupWithDuration) {
    bw_music::UnsafeTrack track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 72)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 84)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));
    addEvent(bw_music::NoteOnEvent(0, 48)); // unterminated and not at the end of the track

    EXPECT_FALSE(bw_music::isTrackValid(track));

    auto builtTrack = trackBuilder.finishAndGetTrack(4);
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(builtTrack.getDuration(), 4);
    EXPECT_EQ(builtTrack.getTotalEventDuration(), 4);
    // Three events added
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents() + 3);
}

TEST(TrackBuilderTest, validBuilder_invalidUnterminatedGroupWithDuration2) {
    bw_music::UnsafeTrack track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    addEvent(bw_music::NoteOnEvent(0, 72)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 60));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 60));
    addEvent(bw_music::NoteOnEvent(0, 62));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 62));
    addEvent(bw_music::NoteOnEvent(0, 84)); // unterminated
    addEvent(bw_music::NoteOnEvent(0, 64));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 64));
    addEvent(bw_music::NoteOnEvent(0, 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 4), 67));
    addEvent(bw_music::NoteOffEvent(babelwires::Rational(3, 1), 50)); // Outside a group, so should be dropped.
    addEvent(bw_music::NoteOnEvent(0, 48)); // unterminated and at the end of the track

    EXPECT_FALSE(bw_music::isTrackValid(track));

    auto builtTrack = trackBuilder.finishAndGetTrack(4);
    EXPECT_FALSE(bw_music::isTrackValid(track));
    EXPECT_TRUE(bw_music::isTrackValid(builtTrack));
    EXPECT_EQ(builtTrack.getDuration(), 4);
    EXPECT_EQ(builtTrack.getTotalEventDuration(), 4);
    // Two events added, two removed
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents());
}

TEST(TrackBuilderTest, validBuilder_InvalidReorderedAndZeroLength) {
    bw_music::UnsafeTrack track;
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getTotalEventDuration());
    EXPECT_EQ(builtTrack.getNumEvents(), track.getNumEvents() - 2);
}

TEST(TrackBuilderTest, validBuilder_InvalidMixture) {
    bw_music::UnsafeTrack goodEvents;
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

    bw_music::UnsafeTrack badEvents;
    badEvents.addEvent(TestEnclosedEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(TestEnclosedEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOnEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOffEvent(0, 72));
    badEvents.addEvent(bw_music::NoteOnEvent(0, 72));

    bw_music::UnsafeTrack track;
    bw_music::TrackBuilder trackBuilder;

    auto addEvent = [&track, &trackBuilder](auto&& event) {
        track.addEvent(event);
        trackBuilder.addEvent(std::forward<decltype(event)>(event));
    };

    // getBadEvent() repeatedly produces badEvents
    auto badEventIt = badEvents.end();
    auto getBadEvent = [&badEventIt, &badEvents]() -> const bw_music::TrackEvent& {
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
    EXPECT_EQ(track.getTotalEventDuration(), builtTrack.getDuration());
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
