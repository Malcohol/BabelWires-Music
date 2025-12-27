#include <gtest/gtest.h>

#include <MusicLib/Utilities/musicUtilities.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

TEST(MusicUtilitiesTest, GetMinimumDenominator) {
    bw_music::TrackBuilder track;
    track.addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 4), 60, 100));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 8), 60));
    track.addEvent(bw_music::NoteOnEvent(babelwires::Rational(1, 16), 62, 100));
    track.addEvent(bw_music::NoteOffEvent(babelwires::Rational(1, 2), 62));

    int minDenom = bw_music::getMinimumDenominator(track.finishAndGetTrack());
    EXPECT_EQ(minDenom, 16);
}

TEST(MusicUtilitiesTest, TransposePitch) {
    using namespace bw_music;

    // Test transposing within range
    auto result = transposePitch(60, 5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 65);

    result = transposePitch(60, 0);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 60);

    result = transposePitch(60, -5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 55);

    // Test transposing below lower limit with Discard policy
    result = transposePitch(2, -5, TransposeOutOfRangePolicy::Discard);
    EXPECT_FALSE(result.has_value());

    // Test transposing above upper limit with Discard policy
    result = transposePitch(126, 5, TransposeOutOfRangePolicy::Discard);
    EXPECT_FALSE(result.has_value());

    // Test transposing below lower limit with MapToNearestOctave policy
    result = transposePitch(2, -5, TransposeOutOfRangePolicy::MapToNearestOctave);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 9); // 2 - 5 = -3; mapped to nearest octave = 9

    // Test transposing above upper limit with MapToNearestOctave policy
    result = transposePitch(126, 5, TransposeOutOfRangePolicy::MapToNearestOctave);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 119); // 126 + 5 = 131; mapped to nearest octave = 119

    /// Non-default lower range
    result = transposePitch(22, -5, TransposeOutOfRangePolicy::MapToNearestOctave, 20, 127);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 29);

    /// Non-default upper range
    result = transposePitch(98, 18, TransposeOutOfRangePolicy::MapToNearestOctave, 0, 100);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 92);
}
