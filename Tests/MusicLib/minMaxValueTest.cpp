#include <gtest/gtest.h>

#include <MusicLib/Utilities/minMaxValue.hpp>

TEST(MinMaxValueTest, UnsignedBitScalingPreservesEndpoints) {
    ASSERT_EQ((bw_music::MinMaxValue<>::assertFromUnsigned<8>(0u).getUnsigned32()), 0u);
    ASSERT_EQ((bw_music::MinMaxValue<>::assertFromUnsigned<8>(0xFFu).getUnsigned32()), 0xFFFFFFFFu);
    ASSERT_EQ((bw_music::MinMaxValue<>::assertFromUnsigned<8>(0x80u).getUnsigned32()), 0x80808080u);
}

TEST(MinMaxValueTest, CanRoundTripSmallerUnsignedDomains) {
    const bw_music::MinMaxValue<> value = bw_music::MinMaxValue<>::assertFromUnsigned<7>(64u);
    EXPECT_EQ(value.getUnsigned<7>(), 64u);
}

TEST(MinMaxValueTest, TryFromUnsignedClampsToMaximum) {
    const bw_music::MinMaxValue<> value = bw_music::MinMaxValue<>::tryFromUnsigned<7>(200u);
    EXPECT_EQ(value.getUnsigned<7>(), 127u);
}

TEST(MinMaxValueTest, NormalizedDoubleConversionsClampAndRoundTrip) {
    EXPECT_DOUBLE_EQ(bw_music::MinMaxValue<>::fromUnsigned32(0u).getNormalizedDouble(), 0.0);
    EXPECT_DOUBLE_EQ(bw_music::MinMaxValue<>::fromUnsigned32(0xFFFFFFFFu).getNormalizedDouble(), 1.0);

    ASSERT_TRUE((bw_music::MinMaxValue<>::fromNormalizedDouble(0.5).has_value()));
    EXPECT_EQ((bw_music::MinMaxValue<>::tryFromNormalizedDouble(-0.5).getUnsigned32()), 0u);
    EXPECT_EQ((bw_music::MinMaxValue<>::tryFromNormalizedDouble(1.5).getUnsigned32()), 0xFFFFFFFFu);
}

TEST(MinMaxValueTest, NarrowStorageUsesWidthSpecificAccessors) {
    const auto value8 = bw_music::MinMaxValue<std::uint8_t>::fromUnsigned16(static_cast<std::uint16_t>(0xABCDu));
    EXPECT_EQ(value8.getUnsigned16(), 0xABABu);
    EXPECT_EQ(value8.getUnsigned8(), 0xABu);
    EXPECT_EQ(value8.getUnsigned32(), 0xABABABABu);

    const auto value16 = bw_music::MinMaxValue<std::uint16_t>::fromUnsigned8(static_cast<std::uint8_t>(0x12u));
    EXPECT_EQ(value16.getUnsigned8(), 0x12u);
    EXPECT_EQ(value16.getUnsigned16(), 0x1212u);
    EXPECT_EQ(value16.getUnsigned32(), 0x12121212u);
}

TEST(MinMaxValueTest, NarrowStorageNormalizedDoubleUsesStorageResolution) {
    const auto value8 = bw_music::MinMaxValue<std::uint8_t>::assertFromNormalizedDouble(0.5);
    EXPECT_EQ(value8.getUnsigned8(), 127u);
    EXPECT_DOUBLE_EQ(value8.getNormalizedDouble(), 127.0 / 255.0);

    const auto value16 = bw_music::MinMaxValue<std::uint16_t>::assertFromNormalizedDouble(0.5);
    EXPECT_EQ(value16.getUnsigned16(), 32767u);
    EXPECT_DOUBLE_EQ(value16.getNormalizedDouble(), 32767.0 / 65535.0);
}