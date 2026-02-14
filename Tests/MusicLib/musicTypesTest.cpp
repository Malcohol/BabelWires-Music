#include <gtest/gtest.h>

#include <MusicLib/musicTypes.hpp>

#include <BaseLib/Utilities/result.hpp>

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

    EXPECT_EQ(bw_music::stringToPitch("C0").value(), 0);
    EXPECT_EQ(bw_music::stringToPitch("C#0").value(), 1);
    EXPECT_EQ(bw_music::stringToPitch("C5").value(), 60);
    EXPECT_EQ(bw_music::stringToPitch("C#5").value(), 61);
    EXPECT_EQ(bw_music::stringToPitch("D5").value(), 62);
    EXPECT_EQ(bw_music::stringToPitch("D#5").value(), 63);
    EXPECT_EQ(bw_music::stringToPitch("E5").value(), 64);
    EXPECT_EQ(bw_music::stringToPitch("F5").value(), 65);
    EXPECT_EQ(bw_music::stringToPitch("F#5").value(), 66);
    EXPECT_EQ(bw_music::stringToPitch("G5").value(), 67);
    EXPECT_EQ(bw_music::stringToPitch("G#5").value(), 68);
    EXPECT_EQ(bw_music::stringToPitch("A5").value(), 69);
    EXPECT_EQ(bw_music::stringToPitch("A#5").value(), 70);
    EXPECT_EQ(bw_music::stringToPitch("B5").value(), 71);
    EXPECT_EQ(bw_music::stringToPitch("C6").value(), 72);
    EXPECT_EQ(bw_music::stringToPitch("G10").value(), 127);

    EXPECT_EQ(bw_music::stringToPitch("  C0  ").value(), 0);

    EXPECT_FALSE(bw_music::stringToPitch("G#10").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("A10").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("A#10").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("B10").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("  ").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C 4").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C #4").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C# 4").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("Z2").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("DA").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C##2").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("G-2").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C11").has_value());
    EXPECT_FALSE(bw_music::stringToPitch("C-1").has_value());
}
