#include <gtest/gtest.h>

#include <MusicLib/Utilities/minCentreMaxValue.hpp>

#include <Tests/TestUtils/resultTestUtils.hpp>

TEST(MinCentreMaxValueTest, FromUnsignedExtremesScaleTo32BitRange) {
    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 minValue7, bw_music::MinCentreMaxValue32::fromUnsigned<7>(0u));
    EXPECT_EQ(minValue7.getUnsigned32(), 0u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 centreValue7, bw_music::MinCentreMaxValue32::fromUnsigned<7>(64u));
    EXPECT_EQ(centreValue7.getUnsigned32(), 0x80000000u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 maxValue7, bw_music::MinCentreMaxValue32::fromUnsigned<7>(127u));
    EXPECT_EQ(maxValue7.getUnsigned32(), 0xFFFFFFFFu);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 minValue14, bw_music::MinCentreMaxValue32::fromUnsigned<14>(0u));
    EXPECT_EQ(minValue14.getUnsigned32(), 0u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 centreValue14, bw_music::MinCentreMaxValue32::fromUnsigned<14>(8192u));
    EXPECT_EQ(centreValue14.getUnsigned32(), 0x80000000u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 maxValue14, bw_music::MinCentreMaxValue32::fromUnsigned<14>(16383u));
    EXPECT_EQ(maxValue14.getUnsigned32(), 0xFFFFFFFFu);
}

TEST(MinCentreMaxValueTest, GetUnsignedRoundTripsMinCentreMaxValues) {
    const auto minValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0u);
    const auto centreValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0x80000000u);
    const auto maxValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0xFFFFFFFFu);

    EXPECT_EQ(minValue.getUnsigned<7>(), 0u);
    EXPECT_EQ(centreValue.getUnsigned<7>(), 64u);
    EXPECT_EQ(maxValue.getUnsigned<7>(), 127u);

    EXPECT_EQ(minValue.getUnsigned<14>(), 0u);
    EXPECT_EQ(centreValue.getUnsigned<14>(), 8192u);
    EXPECT_EQ(maxValue.getUnsigned<14>(), 16383u);
}

TEST(MinCentreMaxValueTest, SignedNormalizedExtremesMapToMinCentreMax) {
    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 minValue, bw_music::MinCentreMaxValue32::fromSignedNormalizedDouble(-1.0));
    EXPECT_EQ(minValue.getUnsigned32(), 0u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 centreValue, bw_music::MinCentreMaxValue32::fromSignedNormalizedDouble(0.0));
    EXPECT_EQ(centreValue.getUnsigned32(), 0x80000000u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::MinCentreMaxValue32 maxValue, bw_music::MinCentreMaxValue32::fromSignedNormalizedDouble(1.0));
    EXPECT_EQ(maxValue.getUnsigned32(), 0xFFFFFFFFu);
}

TEST(MinCentreMaxValueTest, SignedNormalizedRoundTripsExtremesAndCentre) {
    const auto minValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0u);
    const auto centreValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0x80000000u);
    const auto maxValue = bw_music::MinCentreMaxValue32::assertFromUnsigned<32>(0xFFFFFFFFu);

    EXPECT_DOUBLE_EQ(minValue.getSignedNormalizedValue(), -1.0);
    EXPECT_DOUBLE_EQ(centreValue.getSignedNormalizedValue(), 0.0);
    EXPECT_DOUBLE_EQ(maxValue.getSignedNormalizedValue(), 1.0);
}

TEST(MinCentreMaxValueTest, SignedNormalizedTryClampsToNearestValidValue) {
    EXPECT_EQ(bw_music::MinCentreMaxValue32::tryFromSignedNormalizedDouble(-1.01).getUnsigned32(), 0u);
    EXPECT_EQ(bw_music::MinCentreMaxValue32::tryFromSignedNormalizedDouble(1.01).getUnsigned32(), 0xFFFFFFFFu);
}

TEST(MinCentreMaxValueTest, TryFromUnsignedClampsToNearestValidValue) {
    EXPECT_EQ(bw_music::MinCentreMaxValue32::tryFromUnsigned<7>(128u).getUnsigned32(), 0xFFFFFFFFu);
    EXPECT_EQ(bw_music::MinCentreMaxValue16::tryFromUnsigned<14>(16384u).getUnsigned16(), 0xFFFFu);
}

TEST(MinCentreMaxValueTest, RejectsOutOfRangeInputs) {
    EXPECT_FALSE(bw_music::MinCentreMaxValue32::fromUnsigned<7>(128u).has_value());
    EXPECT_FALSE(bw_music::MinCentreMaxValue32::fromUnsigned<14>(16384u).has_value());
    EXPECT_FALSE(bw_music::MinCentreMaxValue32::fromSignedNormalizedDouble(-1.01).has_value());
    EXPECT_FALSE(bw_music::MinCentreMaxValue32::fromSignedNormalizedDouble(1.01).has_value());
}

TEST(MinCentreMaxValueTest, NarrowAndWideStoragePreserveAnchorsWhenProjectedTo32Bit) {
    const auto minValue8 = bw_music::MinCentreMaxValue8::assertFromUnsigned<7>(0u);
    const auto centreValue8 = bw_music::MinCentreMaxValue8::assertFromUnsigned<7>(64u);
    const auto maxValue8 = bw_music::MinCentreMaxValue8::assertFromUnsigned<7>(127u);
    EXPECT_EQ(minValue8.getUnsigned32(), 0u);
    EXPECT_EQ(centreValue8.getUnsigned32(), 0x80000000u);
    EXPECT_EQ(maxValue8.getUnsigned32(), 0xFFFFFFFFu);

    const auto minValue64 = bw_music::MinCentreMaxValue64::assertFromUnsigned<14>(0u);
    const auto centreValue64 = bw_music::MinCentreMaxValue64::assertFromUnsigned<14>(8192u);
    const auto maxValue64 = bw_music::MinCentreMaxValue64::assertFromUnsigned<14>(16383u);
    EXPECT_EQ(minValue64.getUnsigned32(), 0u);
    EXPECT_EQ(centreValue64.getUnsigned32(), 0x80000000u);
    EXPECT_EQ(maxValue64.getUnsigned32(), 0xFFFFFFFFu);
}

TEST(MinCentreMaxValueTest, SupportsUint64StorageAndUnsignedDomains) {
    const auto value64 = bw_music::MinCentreMaxValue64::assertFromUnsigned<64>(0xFEDCBA9876543210ull);
    EXPECT_EQ(value64.getUnsigned64(), 0xFEDCBA9876543210ull);
    EXPECT_EQ(value64.getUnsigned<64>(), 0xFEDCBA9876543210ull);

    const auto centre64 = bw_music::MinCentreMaxValue64::assertFromSignedNormalizedDouble(0.0);
    EXPECT_EQ(centre64.getUnsigned32(), 0x80000000u);
}
