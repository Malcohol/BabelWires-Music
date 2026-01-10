#include <gtest/gtest.h>

#include <BabelWiresLib/Types/Enum/enumValue.hpp>

#include <MusicLib/Types/chordTypeSet.hpp>
#include <MusicLib/libRegistration.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>

TEST(ChordTypeSetTest, getChordTypesFromValue) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    const auto& typeSystem = testEnvironment.m_typeSystem;
    const auto& chordTypeSet = typeSystem.getEntryByType<bw_music::ChordTypeSet>();

    babelwires::ValueHolder chordSetValue = chordTypeSet->createValue(typeSystem);
    ASSERT_TRUE(chordSetValue);

    chordTypeSet->setSize(typeSystem, chordSetValue, 2);

    // Add C Major and A Minor to the set.
    const auto& chordType = typeSystem.getEntryByType<bw_music::ChordType>();
    {
        auto [childValue, _, childType] = chordTypeSet->getChildNonConst(chordSetValue, 0);
        ASSERT_EQ(childType->getTypeExp(), bw_music::ChordType::getThisIdentifier());
        *childValue = babelwires::ValueHolder::makeValue<babelwires::EnumValue>(chordType->getIdentifierFromValue(bw_music::ChordType::Value::m));
    }
    {
        auto [childValue, _, childType] = chordTypeSet->getChildNonConst(chordSetValue, 1);
        ASSERT_EQ(childType->getTypeExp(), bw_music::ChordType::getThisIdentifier());
        *childValue = babelwires::ValueHolder::makeValue<babelwires::EnumValue>(chordType->getIdentifierFromValue(bw_music::ChordType::Value::m7b5));
    }
    
    std::set<bw_music::ChordType::Value> chordTypes =
        chordTypeSet->getChordTypesFromValue(typeSystem, chordSetValue);

    ASSERT_EQ(chordTypes.size(), 2);
    EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::m), chordTypes.end());
    EXPECT_NE(chordTypes.find(bw_music::ChordType::Value::m7b5), chordTypes.end());
}

TEST(ChordTypeSetTest, createValueFromChordTypes) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    const auto& typeSystem = testEnvironment.m_typeSystem;
    const auto& chordTypeSet = typeSystem.getEntryByType<bw_music::ChordTypeSet>();

    std::set<bw_music::ChordType::Value> inputChordTypes = {
        bw_music::ChordType::Value::M,
        bw_music::ChordType::Value::dim,
        bw_music::ChordType::Value::aug
    };

    babelwires::ValueHolder chordSetValue =
        chordTypeSet->createValueFromChordTypes(typeSystem, inputChordTypes);

    std::set<bw_music::ChordType::Value> chordTypes =
        chordTypeSet->getChordTypesFromValue(typeSystem, chordSetValue);

    ASSERT_EQ(chordTypes.size(), inputChordTypes.size());
    for (auto chordType : inputChordTypes) {
        EXPECT_NE(chordTypes.find(chordType), chordTypes.end());
    }
}