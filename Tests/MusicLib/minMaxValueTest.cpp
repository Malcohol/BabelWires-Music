#include <gtest/gtest.h>

#include <MusicLib/Utilities/minMaxValue.hpp>

using bw_music::operator""_mmv16;
using bw_music::operator""_mmv32;

static_assert((0xffff_mmv16).getUnsigned16() == 0xFFFFu);
static_assert((0x12345678_mmv32).getUnsigned32() == 0x12345678u);

TEST(MinMaxValueTest, UnsignedBitScalingPreservesEndpoints) {
    ASSERT_EQ((bw_music::MinMaxValue32::assertFromUnsigned<8>(0u).getUnsigned32()), 0u);
    ASSERT_EQ((bw_music::MinMaxValue32::assertFromUnsigned<8>(0xFFu).getUnsigned32()), 0xFFFFFFFFu);
    ASSERT_EQ((bw_music::MinMaxValue32::assertFromUnsigned<8>(0x80u).getUnsigned32()), 0x80808080u);
}

TEST(MinMaxValueTest, CanRoundTripSmallerUnsignedDomains) {
    const bw_music::MinMaxValue32 value = bw_music::MinMaxValue32::assertFromUnsigned<7>(64u);
    EXPECT_EQ(value.getUnsigned<7>(), 64u);
}

TEST(MinMaxValueTest, TryFromUnsignedClampsToMaximum) {
    const bw_music::MinMaxValue32 value = bw_music::MinMaxValue32::tryFromUnsigned<7>(200u);
    EXPECT_EQ(value.getUnsigned<7>(), 127u);
}

TEST(MinMaxValueTest, NormalizedDoubleConversionsClampAndRoundTrip) {
    EXPECT_DOUBLE_EQ((bw_music::MinMaxValue32::assertFromUnsigned<32>(0u).getNormalizedDouble()), 0.0);
    EXPECT_DOUBLE_EQ((bw_music::MinMaxValue32::assertFromUnsigned<32>(0xFFFFFFFFu).getNormalizedDouble()), 1.0);

    ASSERT_TRUE((bw_music::MinMaxValue32::fromNormalizedDouble(0.5).has_value()));
    EXPECT_EQ((bw_music::MinMaxValue32::tryFromNormalizedDouble(-0.5).getUnsigned32()), 0u);
    EXPECT_EQ((bw_music::MinMaxValue32::tryFromNormalizedDouble(1.5).getUnsigned32()), 0xFFFFFFFFu);
}

TEST(MinMaxValueTest, NarrowStorageUsesWidthSpecificAccessors) {
    const auto value8 = bw_music::MinMaxValue8::assertFromUnsigned<16>(static_cast<std::uint16_t>(0xABCDu));
    EXPECT_EQ(value8.getUnsigned16(), 0xABABu);
    EXPECT_EQ(value8.getUnsigned8(), 0xABu);
    EXPECT_EQ(value8.getUnsigned32(), 0xABABABABu);

    const auto value16 = bw_music::MinMaxValue16::assertFromUnsigned<8>(static_cast<std::uint8_t>(0x12u));
    EXPECT_EQ(value16.getUnsigned8(), 0x12u);
    EXPECT_EQ(value16.getUnsigned16(), 0x1212u);
    EXPECT_EQ(value16.getUnsigned32(), 0x12121212u);
}

TEST(MinMaxValueTest, NarrowStorageNormalizedDoubleUsesStorageResolution) {
    const auto value8 = bw_music::MinMaxValue8::assertFromNormalizedDouble(0.5);
    EXPECT_EQ(value8.getUnsigned8(), 127u);
    EXPECT_DOUBLE_EQ(value8.getNormalizedDouble(), 127.0 / 255.0);

    const auto value16 = bw_music::MinMaxValue16::assertFromNormalizedDouble(0.5);
    EXPECT_EQ(value16.getUnsigned16(), 32767u);
    EXPECT_DOUBLE_EQ(value16.getNormalizedDouble(), 32767.0 / 65535.0);
}

TEST(MinMaxValueTest, SupportsUint64StorageAndUnsignedDomains) {
    const auto value64 = bw_music::MinMaxValue64::assertFromUnsigned<64>(0xFEDCBA9876543210ull);
    EXPECT_EQ(value64.getUnsigned64(), 0xFEDCBA9876543210ull);
    EXPECT_EQ(value64.getUnsigned<64>(), 0xFEDCBA9876543210ull);

    const auto expanded8 = bw_music::MinMaxValue64::assertFromUnsigned<8>(0x80u);
    EXPECT_EQ(expanded8.getUnsigned64(), 0x8080808080808080ull);
}

TEST(MinMaxValueTest, UserDefinedLiteralsCreateExactWidthValues) {
    EXPECT_EQ((0xffff_mmv16).getUnsigned16(), 0xFFFFu);
    EXPECT_EQ((0x12345678_mmv32).getUnsigned32(), 0x12345678u);
}