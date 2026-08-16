#include <gtest/gtest.h>

#include <MusicLib/Utilities/tempoValue.hpp>

#include <BaseLib/Utilities/rounding.hpp>

namespace {
    // Stable a.k.a. round-trip unchanged, meaning that the BPM value can be converted to a TempoValue and back to a BPM
    // value without loss of precision at the specified number of decimal places.
    bool isStableBpm(double bpm, int decimalPlaces) {
        const auto tempoValue = bw_music::TempoValue::fromBpm(bpm);
        return tempoValue.has_value() && (tempoValue->getBpmRounded(decimalPlaces) == bpm);
    }
} // namespace

TEST(TempoValueTest, DefaultValueIs120Bpm) {
    const bw_music::TempoValue defaultTempoValue;
    EXPECT_EQ(defaultTempoValue.getMicrosecondsPerQuaternote(), 500000u);
    EXPECT_DOUBLE_EQ(defaultTempoValue.getBpm(), 120.0);
}

TEST(TempoValueTest, FromBpmAndGettersRoundTripRepresentableValues) {
    const auto tempoValueResult = bw_music::TempoValue::fromBpm(120.0);
    ASSERT_TRUE(tempoValueResult.has_value());
    EXPECT_EQ(tempoValueResult->getMicrosecondsPerQuaternote(), 500000u);
    EXPECT_DOUBLE_EQ(tempoValueResult->getBpm(), 120.0);

    const auto fractionalTempoValueResult = bw_music::TempoValue::fromBpm(133.33);
    ASSERT_TRUE(fractionalTempoValueResult.has_value());
    EXPECT_EQ(fractionalTempoValueResult->getMicrosecondsPerQuaternote(), 450011u);
    EXPECT_DOUBLE_EQ(fractionalTempoValueResult->getBpm(), 60'000'000.0 / 450011.0);
}

TEST(TempoValueTest, FromBpmRejectsInvalidAndOutOfRangeValues) {
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(0.0).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(-120.0).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(std::numeric_limits<double>::quiet_NaN()).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(std::numeric_limits<double>::infinity()).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(120'000'001.0).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromBpm(60'000'000.0 / (0xFFFFFFu + 0.5)).has_value());
}

TEST(TempoValueTest, TryFromBpmClampsToRepresentableRange) {
    EXPECT_EQ(bw_music::TempoValue::tryFromBpm(std::numeric_limits<double>::quiet_NaN()).getMicrosecondsPerQuaternote(),
              500000u);
    EXPECT_EQ(bw_music::TempoValue::tryFromBpm(-1.0).getMicrosecondsPerQuaternote(), 500000u);
    EXPECT_EQ(bw_music::TempoValue::tryFromBpm(120'000'001.0).getMicrosecondsPerQuaternote(), 1u);
    EXPECT_EQ(bw_music::TempoValue::tryFromBpm(60'000'000.0 / (0xFFFFFFu + 0.5)).getMicrosecondsPerQuaternote(),
              0xFFFFFFu);
}

TEST(TempoValueTest, FromMicrosecondsPerQuaternoteAndGettersRoundTripValues) {
    const auto tempoValueResult = bw_music::TempoValue::fromMicrosecondsPerQuaternote(500001u);
    ASSERT_TRUE(tempoValueResult.has_value());
    EXPECT_EQ(tempoValueResult->getMicrosecondsPerQuaternote(), 500001u);
    EXPECT_DOUBLE_EQ(tempoValueResult->getBpm(), 60'000'000.0 / 500001.0);
}

TEST(TempoValueTest, FromMicrosecondsPerQuaternoteRejectsOutOfRangeValues) {
    EXPECT_FALSE(bw_music::TempoValue::fromMicrosecondsPerQuaternote(0u).has_value());
    EXPECT_FALSE(bw_music::TempoValue::fromMicrosecondsPerQuaternote(0x1000000u).has_value());
}

TEST(TempoValueTest, TryFromMicrosecondsPerQuaternoteClampsToRange) {
    EXPECT_EQ(bw_music::TempoValue::tryFromMicrosecondsPerQuaternote(0u).getMicrosecondsPerQuaternote(), 1u);
    EXPECT_EQ(bw_music::TempoValue::tryFromMicrosecondsPerQuaternote(0x1000000u).getMicrosecondsPerQuaternote(),
              0xFFFFFFu);
}

TEST(TempoValueTest, comparison) {
    const auto tempoValue1 = bw_music::TempoValue::assertFromBpm(120);
    const auto tempoValue2 = bw_music::TempoValue::assertFromBpm(140);
    EXPECT_LT(tempoValue1, tempoValue2);
    EXPECT_GT(tempoValue2, tempoValue1);
    EXPECT_EQ(tempoValue1, tempoValue1);
    EXPECT_NE(tempoValue1, tempoValue2);
}

TEST(TempoValueTest, DefaultValueIsStable) {
    for (int decimalPlaces = 0; decimalPlaces <= bw_music::TempoValue::c_maxPrecisionDecimalPlaces; ++decimalPlaces) {
        EXPECT_TRUE(isStableBpm(bw_music::TempoValue().getBpm(), decimalPlaces));
        EXPECT_TRUE(isStableBpm(bw_music::TempoValue().getBpmRounded(decimalPlaces), decimalPlaces));
    }
}

TEST(TempoValueTest, BpmRangeEndpointsAreStable) {
    for (int decimalPlaces = 0; decimalPlaces <= bw_music::TempoValue::c_maxPrecisionDecimalPlaces; ++decimalPlaces) {
        const auto bpmRange = bw_music::TempoValue::getBpmRangeRounded(decimalPlaces);
        const double step = std::pow(10.0, -decimalPlaces);
        const double previousBpm = babelwires::roundTo(bpmRange.m_min - step, decimalPlaces);
        const double nextBpm = babelwires::roundTo(bpmRange.m_max + step, decimalPlaces);

        EXPECT_TRUE(isStableBpm(bpmRange.m_min, decimalPlaces));
        EXPECT_TRUE(isStableBpm(bpmRange.m_max, decimalPlaces));
        EXPECT_FALSE(isStableBpm(previousBpm, decimalPlaces));
        EXPECT_FALSE(isStableBpm(nextBpm, decimalPlaces));
    }
}

TEST(TempoValueTest, RoundedBpmOfRangeAreNotAllValid) {
    // This test shows that rounding the bpm of the actual range does not give a usable bpm range, since some of
    // the rounded values are not representable. For example, the BPM of the range min at 3 decimal places rounds
    // to 3.576, which is not valid.
    const auto actualRange = bw_music::TempoValue::getRange();
    int invalidRoundedEndPoints = 0;
    for (int decimalPlaces = 0; decimalPlaces <= bw_music::TempoValue::c_maxPrecisionDecimalPlaces; ++decimalPlaces) {
        const auto bpmRange = bw_music::TempoValue::getBpmRangeRounded(decimalPlaces);
        // getBpmRounded has a built in clamp, so this test won't fail.
        const auto actualRangeBpmRounded =
            babelwires::Range<double>(babelwires::roundTo(actualRange.m_min.getBpm(), decimalPlaces),
                                      babelwires::roundTo(actualRange.m_max.getBpm(), decimalPlaces));

        if (bw_music::TempoValue::fromBpm(actualRangeBpmRounded.m_min).has_value()) {
            EXPECT_EQ(actualRangeBpmRounded.m_min, bpmRange.m_min);
        } else {
            ++invalidRoundedEndPoints;
        }
        if (bw_music::TempoValue::fromBpm(actualRangeBpmRounded.m_max).has_value()) {
            EXPECT_EQ(actualRangeBpmRounded.m_max, bpmRange.m_max);
        } else {
            ++invalidRoundedEndPoints;
        }
    }
    EXPECT_GT(invalidRoundedEndPoints, 0);
}

TEST(TempoValueTest, BpmStableRangeIsStableAndMaximal) {
    for (int decimalPlaces = 0; decimalPlaces <= bw_music::TempoValue::c_maxStableDecimalPlaces; ++decimalPlaces) {
        const auto bpmRange = bw_music::TempoValue::getBpmStableRangeRounded(decimalPlaces);
        const double step = std::pow(10.0, -decimalPlaces);
        for (double bpm = bpmRange.m_min; bpm <= bpmRange.m_max; bpm += step) {
            const double roundedBpm = babelwires::roundTo(bpm, decimalPlaces);
            // Assert to prevent excessive test output in case of failure, since the range is large and the step is
            // small.
            ASSERT_TRUE(isStableBpm(roundedBpm, decimalPlaces)) << "BPM: " << roundedBpm;
        }
        const double belowLowerRange = babelwires::roundTo(bpmRange.m_min - step, decimalPlaces);
        const double aboveUpperRange = babelwires::roundTo(bpmRange.m_max + step, decimalPlaces);
        EXPECT_FALSE(isStableBpm(belowLowerRange, decimalPlaces)) << "BPM: " << belowLowerRange;
        EXPECT_FALSE(isStableBpm(aboveUpperRange, decimalPlaces)) << "BPM: " << aboveUpperRange;
    }
}

TEST(TempoValueTest, SerializationRoundTrips) {
    const std::array<bw_music::TempoValue, 4> testValues = {
        bw_music::TempoValue(), bw_music::TempoValue::assertFromMicrosecondsPerQuaternote(0x000001u),
        bw_music::TempoValue::assertFromMicrosecondsPerQuaternote(0x0F0F0Fu),
        bw_music::TempoValue::assertFromMicrosecondsPerQuaternote(0xFFFFFFu)};
    for (const auto& tempoValue : testValues) {
        const auto serialized = tempoValue.serializeToString();
        const auto deserializedResult = bw_music::TempoValue::deserializeFromString(serialized);
        ASSERT_TRUE(deserializedResult.has_value());
        EXPECT_EQ(deserializedResult->getMicrosecondsPerQuaternote(), tempoValue.getMicrosecondsPerQuaternote());
    }
}

TEST(TempoValueTest, DeserializationAcceptsValidStrings) {
    const std::array<std::string, 4> invalidStrings = {"0x1", "0x000001", "0xFFFFFF", "0xffffff"};
    for (const auto& str : invalidStrings) {
        const auto deserializedResult = bw_music::TempoValue::deserializeFromString(str);
        EXPECT_TRUE(deserializedResult.has_value()) << "Failure: " << str;
    }
}

TEST(TempoValueTest, DeserializationRejectsInvalidStrings) {
    const std::array<std::string, 5> invalidStrings = {
        "",          "not a number", "0",
        "0x0",       // Valid Hex, but not a tempo value.
        "0x1000000", // Valid Hex, but not a tempo value.
    };
    for (const auto& str : invalidStrings) {
        const auto deserializedResult = bw_music::TempoValue::deserializeFromString(str);
        EXPECT_FALSE(deserializedResult.has_value()) << "Failure: " << str;
    }
}