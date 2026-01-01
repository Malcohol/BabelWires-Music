#include <gtest/gtest.h>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/Types/Track/trackTypeConstructor.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

TEST(TrackType, defaultTrackTypeCreateValue) {
    babelwires::TypeSystem typeSystem;
    bw_music::DefaultTrackType trackType;

    babelwires::ValueHolder newValue = trackType.createValue(typeSystem);
    EXPECT_TRUE(newValue);

    const auto* const newTrackValue = newValue->as<bw_music::Track>();
    EXPECT_NE(newTrackValue, nullptr);
    EXPECT_EQ(newTrackValue->getDuration(), 0);
}

TEST(TrackType, defaultTrackTypeIsValidValue) {
    babelwires::TypeSystem typeSystem;
    bw_music::DefaultTrackType trackType;

    bw_music::Track track(8);

    EXPECT_TRUE(trackType.isValidValue(typeSystem, track));
    EXPECT_FALSE(trackType.isValidValue(typeSystem, babelwires::RationalValue(3)));
}

TEST(TrackType, defaultTrackGetKindIsSet) {
    bw_music::DefaultTrackType trackType;

    EXPECT_FALSE(trackType.getFlavour().empty());
}

TEST(TrackType, constructedTrackTypeCreateValue) {
    testUtils::TestEnvironment testEnvironment;
    testEnvironment.m_typeSystem.addTypeConstructor<bw_music::TrackTypeConstructor>();

    babelwires::TypeExp trackTypeRef(bw_music::TrackTypeConstructor::getThisIdentifier(), babelwires::RationalValue(8));

    const babelwires::TypePtr& type = trackTypeRef.tryResolve(testEnvironment.m_typeSystem);

    babelwires::ValueHolder newValue = type->createValue(testEnvironment.m_typeSystem);
    EXPECT_TRUE(newValue);

    const auto* const newTrack = newValue->as<bw_music::Track>();
    EXPECT_NE(newTrack, nullptr);
    EXPECT_EQ(newTrack->getDuration(), 8);
}