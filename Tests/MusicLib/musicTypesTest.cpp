#include <gtest/gtest.h>

#include <MusicLib/musicTypes.hpp>

#include <BaseLib/exceptions.hpp>

TEST(MusicLib, enumNames) {
    EXPECT_EQ(bw_music::PitchClass::valueToString(bw_music::PitchClass::Value::C), "C");

    EXPECT_EQ(bw_music::pitchToString(0), "C0");
    EXPECT_EQ(bw_music::pitchToString(1), "C#0");
    EXPECT_EQ(bw_music::pitchToString(60), "C5");
    EXPECT_EQ(bw_music::pitchToString(61), "C#5");
    EXPECT_EQ(bw_music::pitchToString(62), "D5");
    EXPECT_EQ(bw_music::pitchToString(63), "D#5");
    EXPECT_EQ(bw_music::pitchToString(64), "E5");
    EXPECT_EQ(bw_music::pitchToString(65), "F5");
    EXPECT_EQ(bw_music::pitchToString(66), "F#5");
    EXPECT_EQ(bw_music::pitchToString(67), "G5");
    EXPECT_EQ(bw_music::pitchToString(68), "G#5");
    EXPECT_EQ(bw_music::pitchToString(69), "A5");
    EXPECT_EQ(bw_music::pitchToString(70), "A#5");
    EXPECT_EQ(bw_music::pitchToString(71), "B5");
    EXPECT_EQ(bw_music::pitchToString(72), "C6");
    EXPECT_EQ(bw_music::pitchToString(127), "G10");

    EXPECT_EQ(bw_music::stringToPitch("C0"), 0);
    EXPECT_EQ(bw_music::stringToPitch("C#0"), 1);
    EXPECT_EQ(bw_music::stringToPitch("C5"), 60);
    EXPECT_EQ(bw_music::stringToPitch("C#5"), 61);
    EXPECT_EQ(bw_music::stringToPitch("D5"), 62);
    EXPECT_EQ(bw_music::stringToPitch("D#5"), 63);
    EXPECT_EQ(bw_music::stringToPitch("E5"), 64);
    EXPECT_EQ(bw_music::stringToPitch("F5"), 65);
    EXPECT_EQ(bw_music::stringToPitch("F#5"), 66);
    EXPECT_EQ(bw_music::stringToPitch("G5"), 67);
    EXPECT_EQ(bw_music::stringToPitch("G#5"), 68);
    EXPECT_EQ(bw_music::stringToPitch("A5"), 69);
    EXPECT_EQ(bw_music::stringToPitch("A#5"), 70);
    EXPECT_EQ(bw_music::stringToPitch("B5"), 71);
    EXPECT_EQ(bw_music::stringToPitch("C6"), 72);
    EXPECT_EQ(bw_music::stringToPitch("G10"), 127);

    EXPECT_EQ(bw_music::stringToPitch("  C0  "), 0);

    EXPECT_THROW(bw_music::stringToPitch("G#10"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("A10"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("A#10"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("B10"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch(""), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("  "), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C 4"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C #4"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C# 4"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("Z2"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("DA"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C##2"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("G-2"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C11"), babelwires::ParseException);
    EXPECT_THROW(bw_music::stringToPitch("C-1"), babelwires::ParseException);
}
