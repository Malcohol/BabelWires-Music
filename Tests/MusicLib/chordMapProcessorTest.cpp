#include <gtest/gtest.h>

#include <MusicLib/Functions/mapChordsFunction.hpp>
#include <MusicLib/Processors/chordMapProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/chord.hpp>
#include <MusicLib/libRegistration.hpp>
#include <MusicLib/Functions/mergeFunction.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToOneFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/Types/Tuple/tupleValue.hpp>
#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    enum class SourceMode { ChordToChord, SilenceToChord };
    enum class TargetMode { ChordToChord, ChordToSilence };
    enum class WildcardMode { NoWildcards, Wildcards };

    babelwires::MapValue getTestChordMap(const babelwires::TypeSystem& typeSystem, SourceMode sourceMode,
                                         TargetMode targetMode, WildcardMode wildcardMode) {
        babelwires::TypePtrT<bw_music::ChordType> chordTypeEnum = typeSystem.getRegisteredType<bw_music::ChordType>();
        babelwires::TypePtrT<bw_music::PitchClass> pitchClassEnum =
            typeSystem.getRegisteredType<bw_music::PitchClass>();

        babelwires::MapValue chordMap;
        chordMap.setSourceTypeExp(bw_music::getMapChordFunctionSourceTypeExp());
        chordMap.setTargetTypeExp(bw_music::getMapChordFunctionTargetTypeExp());

        auto sourceType = chordMap.getSourceTypeExp().assertResolve(typeSystem);
        auto targetType = chordMap.getTargetTypeExp().assertResolve(typeSystem);

        // This object gets re-used.
        babelwires::OneToOneMapEntryData chordMaplet(typeSystem, *sourceType, *targetType);
        {
            {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum->getIdentifierFromValue(bw_music::PitchClass::Value::D));
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::m));
                babelwires::TupleValue sourceValue({pitchClassSourceValue.clone(), chordTypeSourceValue.clone()});

                babelwires::EnumValue pitchClassTargetValue(
                    pitchClassEnum->getIdentifierFromValue(bw_music::PitchClass::Value::A));
                babelwires::EnumValue chordTypeTargetValue(
                    chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::m7));
                babelwires::TupleValue targetValue({pitchClassTargetValue.clone(), chordTypeTargetValue.clone()});

                chordMaplet.setSourceValue(sourceValue.clone());
                chordMaplet.setTargetValue(targetValue.clone());
                chordMap.emplaceBack(chordMaplet.clone());
            }
            if (wildcardMode == WildcardMode::Wildcards) {
                babelwires::EnumValue wildcardValue(babelwires::getWildcardId());
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::M7s11));
                babelwires::TupleValue sourceValue({wildcardValue.clone(), chordTypeSourceValue.clone()});

                babelwires::EnumValue wildcardMatchValue(babelwires::getWildcardMatchId());
                babelwires::EnumValue chordTypeTargetValue(
                    chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::m7));
                babelwires::TupleValue targetValue({wildcardMatchValue.clone(), chordTypeTargetValue.clone()});

                chordMaplet.setSourceValue(sourceValue.clone());
                chordMaplet.setTargetValue(targetValue.clone());
                chordMap.emplaceBack(chordMaplet.clone());
            }
        }
        if (sourceMode == SourceMode::SilenceToChord) {
            babelwires::EnumValue sourceValue(bw_music::NoChord::getNoChordValue());

            babelwires::EnumValue pitchClassTargetValue(
                pitchClassEnum->getIdentifierFromValue(bw_music::PitchClass::Value::Fsh));
            babelwires::EnumValue chordTypeTargetValue(
                chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::m7_11));
            babelwires::TupleValue targetValue({pitchClassTargetValue.clone(), chordTypeTargetValue.clone()});

            chordMaplet.setSourceValue(sourceValue.clone());
            chordMaplet.setTargetValue(targetValue.clone());
            chordMap.emplaceBack(chordMaplet.clone());
        }
        if (targetMode == TargetMode::ChordToSilence) {
            {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum->getIdentifierFromValue(bw_music::PitchClass::Value::Gsh));
                babelwires::EnumValue chordTypeSourceValue(
                    chordTypeEnum->getIdentifierFromValue(bw_music::ChordType::Value::M6));
                babelwires::TupleValue sourceValue({pitchClassSourceValue.clone(), chordTypeSourceValue.clone()});

                babelwires::EnumValue targetValue;
                targetValue.set(bw_music::NoChord::getNoChordValue());

                chordMaplet.setSourceValue(sourceValue.clone());
                chordMaplet.setTargetValue(targetValue.clone());
                chordMap.emplaceBack(chordMaplet.clone());
            }
            if (wildcardMode == WildcardMode::Wildcards) {
                babelwires::EnumValue pitchClassSourceValue(
                    pitchClassEnum->getIdentifierFromValue(bw_music::PitchClass::Value::Dsh));
                babelwires::EnumValue wildcardValue(babelwires::getWildcardId());
                babelwires::TupleValue sourceValue({pitchClassSourceValue.clone(), wildcardValue.clone()});

                babelwires::EnumValue targetValue;
                targetValue.set(bw_music::NoChord::getNoChordValue());

                chordMaplet.setSourceValue(sourceValue.clone());
                chordMaplet.setTargetValue(targetValue.clone());
                chordMap.emplaceBack(chordMaplet.clone());
            }
        }
        {
            babelwires::AllToOneFallbackMapEntryData chordMaplet(typeSystem, *targetType);

            babelwires::EnumValue wildcardValue(babelwires::getWildcardMatchId());
            babelwires::TupleValue targetValue({wildcardValue.clone(), wildcardValue.clone()});

            chordMaplet.setTargetValue(targetValue.clone());

            chordMap.emplaceBack(chordMaplet.clone());
        }
        return chordMap;
    }

    bw_music::Track getTestInputTrack() {
        bw_music::TrackBuilder track;

        testUtils::addChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M,
                               babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                              {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m},
                              {bw_music::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::M6},
                              {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M},
                              {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                              {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m,
                               babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                              {bw_music::PitchClass::Value::D, bw_music::ChordType::ChordType::Value::m},
                              {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M7s11}},
                             track);

        return track.finishAndGetTrack(bw_music::ModelDuration(11, 2));
    }

    void testOutputTrack(const bw_music::Track& outputTrack, SourceMode sourceMode, TargetMode targetMode,
                         WildcardMode wildcardMode) {
        EXPECT_EQ(outputTrack.getDuration(), babelwires::Rational(11, 2));

        if (wildcardMode == WildcardMode::NoWildcards) {
            if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::M6},
                                       {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M},
                                       {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                                       {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M7s11}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                                       {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M7s11}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords(
                    {{bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::M6},
                     {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M7s11},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords(
                    {{bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M,
                      babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::M7s11},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            }
        } else {
            if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::M6},
                                       {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M},
                                       {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                                       {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::m7}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::ChordToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords({{bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1)},
                                       {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m,
                                        babelwires::Rational(1, 2), babelwires::Rational(1, 2)},
                                       {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                                       {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::m7}},
                                      outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToChord)) {
                testUtils::testChords(
                    {{bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Gsh, bw_music::ChordType::ChordType::Value::M6},
                     {bw_music::PitchClass::Value::Dsh, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11}},
                    outputTrack);
            } else if ((sourceMode == SourceMode::SilenceToChord) && (targetMode == TargetMode::ChordToSilence)) {
                testUtils::testChords(
                    {{bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::M},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::C, bw_music::ChordType::ChordType::Value::m,
                      babelwires::Rational(1, 2), babelwires::Rational(1)},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     {bw_music::PitchClass::Value::E, bw_music::ChordType::ChordType::Value::m},
                     {bw_music::PitchClass::Value::A, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Csh, bw_music::ChordType::ChordType::Value::m7},
                     {bw_music::PitchClass::Value::Fsh, bw_music::ChordType::ChordType::Value::m7_11},
                     },
                    outputTrack);
            }
        }
    }

    using TestData = std::tuple<SourceMode, TargetMode, WildcardMode>;
} // namespace

class ChordMapProcessorTest : public testing::TestWithParam<TestData> {};

TEST_P(ChordMapProcessorTest, simpleFunction) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    const auto [sourceMode, targetMode, wildcardMode] = GetParam();
    babelwires::MapValue chordMap = getTestChordMap(testEnvironment.m_typeSystem, sourceMode, targetMode, wildcardMode);

    bw_music::Track inputTrack = getTestInputTrack();
    bw_music::Track outputTrack = bw_music::mapChordsFunction(testEnvironment.m_typeSystem, inputTrack, chordMap);
    testOutputTrack(outputTrack, sourceMode, targetMode, wildcardMode);
}

INSTANTIATE_TEST_SUITE_P(
    ChordMapProcessorTest, ChordMapProcessorTest,
    testing::Values(TestData{SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToSilence, WildcardMode::NoWildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::NoWildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::Wildcards},
                    TestData{SourceMode::ChordToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToChord, WildcardMode::Wildcards},
                    TestData{SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards}));


TEST(ChordMapProcessorTest, chordsAndNotes) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    babelwires::MapValue chordMap = getTestChordMap(testEnvironment.m_typeSystem, SourceMode::SilenceToChord, TargetMode::ChordToSilence, WildcardMode::Wildcards);

    bw_music::Track chordTrack = getTestInputTrack();

    bw_music::TrackBuilder noteTrackBuilder;

    testUtils::addNotes(
    {
        {60, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {62, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {64, babelwires::Rational(3, 4), babelwires::Rational(0, 4)},
        {65, babelwires::Rational(3, 4), babelwires::Rational(0, 4)},
        {67, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {69, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
        {71, babelwires::Rational(3, 4), babelwires::Rational(0, 4)},
        {72, babelwires::Rational(1, 4), babelwires::Rational(1, 4)},
    }, noteTrackBuilder);
    bw_music::Track noteTrack = noteTrackBuilder.finishAndGetTrack();

    bw_music::Track preCombinedInput = bw_music::mergeTracks({&noteTrack, &chordTrack});
    bw_music::Track preCombinedOutput = bw_music::mapChordsFunction(testEnvironment.m_typeSystem, preCombinedInput, chordMap);

    bw_music::Track mappedChords = bw_music::mapChordsFunction(testEnvironment.m_typeSystem, chordTrack, chordMap);
    bw_music::Track postCombinedOutput = bw_music::mergeTracks({&noteTrack, &mappedChords});

    // This test exploits the fact that the the resulting events end up in the same relative order.
    // That's not guaranteed, and the test may have to sanitize both sides if that changes.
    EXPECT_EQ(preCombinedOutput, postCombinedOutput);
}

TEST(ChordMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    babelwires::TypePtrT<bw_music::ChordType> chordTypeEnum =
        testEnvironment.m_typeSystem.getRegisteredType<bw_music::ChordType>();
    babelwires::TypePtrT<bw_music::PitchClass> pitchClassEnum =
        testEnvironment.m_typeSystem.getRegisteredType<bw_music::PitchClass>();

    bw_music::ChordMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(bw_music::ChordMapProcessor::getCommonArrayId())
            .as<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(bw_music::ChordMapProcessor::getCommonArrayId())
            .as<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(outputArray);

    bw_music::ChordMapProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);

    in.getChrdMp()->setValue(getTestChordMap(testEnvironment.m_typeSystem, SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards));
    inArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    testOutputTrack(outArray.getEntry(0).get(), SourceMode::ChordToChord, TargetMode::ChordToChord, WildcardMode::NoWildcards);
}

