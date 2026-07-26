#include <gtest/gtest.h>

#include <MusicLib/Utilities/asymmetricCentredInt.hpp>

#include <Tests/TestUtils/resultTestUtils.hpp>

TEST(AsymmetricCentredIntTest, FromUnsignedExtremesScaleTo32BitRange) {
    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt minValue7, bw_music::AsymmetricCentredInt::fromUnsigned<7>(0));
    EXPECT_EQ(minValue7.getUnsigned32(), 0u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt centreValue7, bw_music::AsymmetricCentredInt::fromUnsigned<7>(64));
    EXPECT_EQ(centreValue7.getUnsigned32(), 0x80000000u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt maxValue7, bw_music::AsymmetricCentredInt::fromUnsigned<7>(127));
    EXPECT_EQ(maxValue7.getUnsigned32(), 0xFFFFFFFFu);
    
    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt minValue14, bw_music::AsymmetricCentredInt::fromUnsigned<14>(0));
    EXPECT_EQ(minValue14.getUnsigned32(), 0u);
    
    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt centreValue14, bw_music::AsymmetricCentredInt::fromUnsigned<14>(8192));
    EXPECT_EQ(centreValue14.getUnsigned32(), 0x80000000u);
    
    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt maxValue14, bw_music::AsymmetricCentredInt::fromUnsigned<14>(16383));
    EXPECT_EQ(maxValue14.getUnsigned32(), 0xFFFFFFFFu);
}

TEST(AsymmetricCentredIntTest, GetUnsignedRoundTripsMinCentreMaxValues) {
    const auto minValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0u);
    const auto centreValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0x80000000u);
    const auto maxValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0xFFFFFFFFu);

    EXPECT_EQ(minValue.getUnsigned<7>(), 0);
    EXPECT_EQ(centreValue.getUnsigned<7>(), 64);
    EXPECT_EQ(maxValue.getUnsigned<7>(), 127);

    EXPECT_EQ(minValue.getUnsigned<14>(), 0u);
    EXPECT_EQ(centreValue.getUnsigned<14>(), 8192u);
    EXPECT_EQ(maxValue.getUnsigned<14>(), 16383u);
}

TEST(AsymmetricCentredIntTest, SignedNormalizedExtremesMapToMinCentreMax) {
    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt minValue, bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(-1.0));
    EXPECT_EQ(minValue.getUnsigned32(), 0u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt centreValue, bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(0.0));
    EXPECT_EQ(centreValue.getUnsigned32(), 0x80000000u);

    BW_ASSERT_RESULT_ASSIGN(bw_music::AsymmetricCentredInt maxValue, bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(1.0));
    EXPECT_EQ(maxValue.getUnsigned32(), 0xFFFFFFFFu);
}

TEST(AsymmetricCentredIntTest, SignedNormalizedRoundTripsExtremesAndCentre) {
    const auto minValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0u);
    const auto centreValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0x80000000u);
    const auto maxValue = bw_music::AsymmetricCentredInt::fromUnsigned32(0xFFFFFFFFu);

    EXPECT_DOUBLE_EQ(minValue.getSignedNormalizedValue(), -1.0);
    EXPECT_DOUBLE_EQ(centreValue.getSignedNormalizedValue(), 0.0);
    EXPECT_DOUBLE_EQ(maxValue.getSignedNormalizedValue(), 1.0);
}

TEST(AsymmetricCentredIntTest, SignedNormalizedTryClampsToNearestValidValue) {
    EXPECT_EQ(bw_music::AsymmetricCentredInt::tryFromSignedNormalizedDouble(-1.01).getUnsigned32(), 0u);
    EXPECT_EQ(bw_music::AsymmetricCentredInt::tryFromSignedNormalizedDouble(1.01).getUnsigned32(), 0xFFFFFFFFu);
}

TEST(AsymmetricCentredIntTest, RejectsOutOfRangeInputs) {
    EXPECT_FALSE(bw_music::AsymmetricCentredInt::fromUnsigned<7>(128).has_value());
    EXPECT_FALSE(bw_music::AsymmetricCentredInt::fromUnsigned<14>(16384).has_value());
    EXPECT_FALSE(bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(-1.01).has_value());
    EXPECT_FALSE(bw_music::AsymmetricCentredInt::fromSignedNormalizedDouble(1.01).has_value());
}
