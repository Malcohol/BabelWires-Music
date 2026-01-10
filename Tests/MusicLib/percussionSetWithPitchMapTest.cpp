#include <gtest/gtest.h>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/equalSets.hpp>
#include <Tests/TestUtils/testIdentifiers.hpp>

namespace {
    struct TestPercussionSet : bw_music::PercussionSetWithPitchMap
    {
        TestPercussionSet(InstrumentBlock block, bw_music::Pitch pitchOfDefaultValue) : PercussionSetWithPitchMap(testUtils::getTestRegisteredMediumIdentifier("Foo"), std::move(block), pitchOfDefaultValue) {}
        TestPercussionSet(std::vector<InstrumentBlock> instruments, bw_music::Pitch pitchOfDefaultValue) : PercussionSetWithPitchMap(testUtils::getTestRegisteredMediumIdentifier("Foo"), std::move(instruments), pitchOfDefaultValue) {}
    };
}

TEST(PercussionSetWithPitchMapTest, oneInstrumentBlockAllBuiltIn) {
    testUtils::TestEnvironment testEnvironment;
    const bw_music::BuiltInPercussionInstruments* const builtIns =
        testEnvironment.m_typeSystem.addEntry<bw_music::BuiltInPercussionInstruments>();

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block = {{bw_music::BuiltInPercussionInstruments::Value::HBongo,
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves,
                                                       bw_music::BuiltInPercussionInstruments::Value::Bass1},
                                                      40,
                                                      builtIns};

    TestPercussionSet percussionSet(block, 41);

    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getValueSet(), {"HBongo", "Claves", "Bass1"}));
    EXPECT_EQ(percussionSet.getValueSet().size(), 3);
    EXPECT_EQ(percussionSet.getValueSet()[0], "HBongo");
    EXPECT_EQ(percussionSet.getValueSet()[1], "Claves");
    EXPECT_EQ(percussionSet.getValueSet()[2], "Bass1");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("HBongo"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Claves"), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bass1"), 2);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("HBongo"), 40);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Claves"), 41);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bass1"), 42);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(40), "HBongo");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(41), "Claves");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(42), "Bass1");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(39));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(43));
}

TEST(PercussionSetWithPitchMapTest, oneInstrumentBlockAllNew) {
    // No need to supply a point to BuiltInPercussionInstruments in this case.
    bw_music::PercussionSetWithPitchMap::InstrumentBlock block = {{testUtils::getTestRegisteredIdentifier("Bar"),
                                                       testUtils::getTestRegisteredIdentifier("Boo")},
                                                      50};

    TestPercussionSet percussionSet(block, 50);

    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getValueSet(), {"Bar", "Boo"}));
    EXPECT_EQ(percussionSet.getValueSet().size(), 2);
    EXPECT_EQ(percussionSet.getValueSet()[0], "Bar");
    EXPECT_EQ(percussionSet.getValueSet()[1], "Boo");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bar"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Boo"), 1);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bar"), 50);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Boo"), 51);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(50), "Bar");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(51), "Boo");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(49));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(52));
}

TEST(PercussionSetWithPitchMapTest, oneInstrumentBlockMixed) {
    testUtils::TestEnvironment testEnvironment;
    const bw_music::BuiltInPercussionInstruments* const builtIns =
        testEnvironment.m_typeSystem.addEntry<bw_music::BuiltInPercussionInstruments>();

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block = {{testUtils::getTestRegisteredIdentifier("Bar"),
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves},
                                                      50,
                                                      builtIns};

    TestPercussionSet percussionSet(block, 50);

    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getValueSet(), {"Bar", "Claves"}));
    EXPECT_EQ(percussionSet.getValueSet().size(), 2);
    EXPECT_EQ(percussionSet.getValueSet()[0], "Bar");
    EXPECT_EQ(percussionSet.getValueSet()[1], "Claves");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bar"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Claves"), 1);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bar"), 50);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Claves"), 51);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(50), "Bar");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(51), "Claves");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(49));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(52));
}

TEST(PercussionSetWithPitchMapTest, twoInstrumentBlocks) {
    testUtils::TestEnvironment testEnvironment;
    const bw_music::BuiltInPercussionInstruments* const builtIns =
        testEnvironment.m_typeSystem.addEntry<bw_music::BuiltInPercussionInstruments>();

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block0 = {{bw_music::BuiltInPercussionInstruments::Value::HBongo,
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves,
                                                       bw_music::BuiltInPercussionInstruments::Value::Bass1},
                                                      40,
                                                      builtIns};

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block1 = {{testUtils::getTestRegisteredIdentifier("Bar"),
                                                       testUtils::getTestRegisteredIdentifier("Boo")},
                                                      50};

    TestPercussionSet percussionSet({ block0, block1 }, 41);

    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getValueSet(), {"HBongo", "Claves", "Bass1", "Bar", "Boo"}));
    EXPECT_EQ(percussionSet.getValueSet().size(), 5);
    EXPECT_EQ(percussionSet.getValueSet()[0], "HBongo");
    EXPECT_EQ(percussionSet.getValueSet()[1], "Claves");
    EXPECT_EQ(percussionSet.getValueSet()[2], "Bass1");
    EXPECT_EQ(percussionSet.getValueSet()[3], "Bar");
    EXPECT_EQ(percussionSet.getValueSet()[4], "Boo");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("HBongo"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Claves"), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bass1"), 2);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bar"), 3);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Boo"), 4);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("HBongo"), 40);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Claves"), 41);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bass1"), 42);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bar"), 50);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Boo"), 51);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(40), "HBongo");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(41), "Claves");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(42), "Bass1");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(50), "Bar");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(51), "Boo");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(39));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(43));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(49));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(52));
}

TEST(PercussionSetWithPitchMapTest, duplicates) {
    testUtils::TestEnvironment testEnvironment;
    const bw_music::BuiltInPercussionInstruments* const builtIns =
        testEnvironment.m_typeSystem.addEntry<bw_music::BuiltInPercussionInstruments>();

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block0 = {{bw_music::BuiltInPercussionInstruments::Value::HBongo,
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves,
                                                       bw_music::BuiltInPercussionInstruments::Value::Bass1,
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves},
                                                      40,
                                                      builtIns};

    bw_music::PercussionSetWithPitchMap::InstrumentBlock block1 = {{testUtils::getTestRegisteredIdentifier("Bar"),
                                                       bw_music::BuiltInPercussionInstruments::Value::Claves},
                                                      50,
                                                      builtIns};

    TestPercussionSet percussionSet({ block0, block1 }, 41);

    EXPECT_TRUE(testUtils::areEqualSets(percussionSet.getValueSet(), {"HBongo", "Claves", "Bass1", "Bar"}));
    EXPECT_EQ(percussionSet.getValueSet().size(), 4);
    EXPECT_EQ(percussionSet.getValueSet()[0], "HBongo");
    EXPECT_EQ(percussionSet.getValueSet()[1], "Claves");
    EXPECT_EQ(percussionSet.getValueSet()[2], "Bass1");
    EXPECT_EQ(percussionSet.getValueSet()[3], "Bar");
    EXPECT_EQ(percussionSet.getIndexOfDefaultValue(), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("HBongo"), 0);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Claves"), 1);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bass1"), 2);
    EXPECT_EQ(percussionSet.tryGetIndexFromIdentifier("Bar"), 3);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("HBongo"), 40);
    // The lowest of the duplicate pitches is dominant.
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Claves"), 41);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bass1"), 42);
    EXPECT_EQ(percussionSet.tryGetPitchFromInstrument("Bar"), 50);
    EXPECT_FALSE(percussionSet.tryGetPitchFromInstrument("Flerm"));
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(40), "HBongo");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(41), "Claves");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(42), "Bass1");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(50), "Bar");
    // Duplicates can still be found using pitch.
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(43), "Claves");
    EXPECT_EQ(percussionSet.tryGetInstrumentFromPitch(51), "Claves");
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(39));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(44));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(49));
    EXPECT_FALSE(percussionSet.tryGetInstrumentFromPitch(52));
}
