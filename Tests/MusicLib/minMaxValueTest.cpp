#include <gtest/gtest.h>

#include <MusicLib/Utilities/minMaxValue.hpp>

TEST(MinMaxValueTest, UnsignedBitScalingPreservesEndpoints) {
    ASSERT_EQ(bw_music::MinMaxValue::assertFromUnsigned<8>(0u).getUnsigned32(), 0u);
    ASSERT_EQ(bw_music::MinMaxValue::assertFromUnsigned<8>(0xFFu).getUnsigned32(), 0xFFFFFFFFu);
    ASSERT_EQ(bw_music::MinMaxValue::assertFromUnsigned<8>(0x80u).getUnsigned32(), 0x80808080u);
}

TEST(MinMaxValueTest, CanRoundTripSmallerUnsignedDomains) {
    const bw_music::MinMaxValue value = bw_music::MinMaxValue::assertFromUnsigned<7>(64u);
    EXPECT_EQ(value.getUnsigned<7>(), 64u);
}

TEST(MinMaxValueTest, TryFromUnsignedClampsToMaximum) {
    const bw_music::MinMaxValue value = bw_music::MinMaxValue::tryFromUnsigned<7>(200u);
    EXPECT_EQ(value.getUnsigned<7>(), 127u);
}

TEST(MinMaxValueTest, NormalizedDoubleConversionsClampAndRoundTrip) {
    EXPECT_DOUBLE_EQ(bw_music::MinMaxValue::fromUnsigned32(0u).getNormalizedDouble(), 0.0);
    EXPECT_DOUBLE_EQ(bw_music::MinMaxValue::fromUnsigned32(0xFFFFFFFFu).getNormalizedDouble(), 1.0);

    ASSERT_TRUE(bw_music::MinMaxValue::fromNormalizedDouble(0.5).has_value());
    EXPECT_EQ(bw_music::MinMaxValue::tryFromNormalizedDouble(-0.5).getUnsigned32(), 0u);
    EXPECT_EQ(bw_music::MinMaxValue::tryFromNormalizedDouble(1.5).getUnsigned32(), 0xFFFFFFFFu);
}