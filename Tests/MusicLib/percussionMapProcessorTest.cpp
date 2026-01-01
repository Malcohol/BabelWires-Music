#include <gtest/gtest.h>

#include <MusicLib/Functions/percussionMapFunction.hpp>
#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Processors/percussionMapProcessor.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/libRegistration.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Enum/enumAtomTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumUnionTypeConstructor.hpp>
#include <BabelWiresLib/Types/Enum/enumValue.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/allToSameFallbackMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/MapEntries/oneToOneMapEntryData.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>
#include <BabelWiresLib/Types/Map/standardMapIdentifiers.hpp>
#include <BabelWiresLib/ValueTree/valueTreeRoot.hpp>

#include <Tests/BabelWiresLib/TestUtils/testEnvironment.hpp>
#include <Tests/TestUtils/seqTestUtils.hpp>
#include <Tests/TestUtils/testLog.hpp>

namespace {
    babelwires::MapValue getTestPercussionMap(const babelwires::TypeSystem& typeSystem) {
        const auto& builtInPercussion =
            typeSystem.getEntryByType<bw_music::BuiltInPercussionInstruments>();

        babelwires::TypeExp targetTypeRef =
            babelwires::EnumUnionTypeConstructor::makeTypeExp(bw_music::BuiltInPercussionInstruments::getThisType(),
            babelwires::EnumAtomTypeConstructor::makeTypeExp(babelwires::getBlankValueId()));

        babelwires::MapValue percussionMap;
        percussionMap.setSourceTypeRef(bw_music::BuiltInPercussionInstruments::getThisType());
        percussionMap.setTargetTypeRef(targetTypeRef);

        babelwires::OneToOneMapEntryData maplet(typeSystem, bw_music::BuiltInPercussionInstruments::getThisType(),
                                                targetTypeRef);

        babelwires::EnumValue sourceValue;
        babelwires::EnumValue targetValue;

        sourceValue.set(builtInPercussion->getIdentifierFromValue(bw_music::BuiltInPercussionInstruments::Value::Clap));
        targetValue.set(
            builtInPercussion->getIdentifierFromValue(bw_music::BuiltInPercussionInstruments::Value::Cowbll));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion->getIdentifierFromValue(bw_music::BuiltInPercussionInstruments::Value::Crash1));
        targetValue.set(
            builtInPercussion->getIdentifierFromValue(bw_music::BuiltInPercussionInstruments::Value::Crash2));
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        sourceValue.set(
            builtInPercussion->getIdentifierFromValue(bw_music::BuiltInPercussionInstruments::Value::LFlTom));
        targetValue.set(babelwires::getBlankValueId());
        maplet.setSourceValue(sourceValue.clone());
        maplet.setTargetValue(targetValue.clone());
        percussionMap.emplaceBack(maplet.clone());

        percussionMap.emplaceBack(std::make_unique<babelwires::AllToSameFallbackMapEntryData>());
        return percussionMap;
    }

    bw_music::Track getTestInputTrack() {
        bw_music::TrackBuilder track;
        track.addEvent(bw_music::PercussionOnEvent{0, "AcBass", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "AcBass", 64});
        track.addEvent(bw_music::PercussionOnEvent{0, "Clap", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Clap", 64});
        track.addEvent(bw_music::PercussionOnEvent{0, "LFlTom", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "LFlTom", 64});
        // An unrecognized instrument.
        track.addEvent(bw_music::PercussionOnEvent{0, "Dummy", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(bw_music::PercussionOnEvent{0, "Crash1", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Crash1", 64});
        return track.finishAndGetTrack();
    }

    bw_music::Track getTestOutputTrack() {
        bw_music::TrackBuilder track;
        track.addEvent(bw_music::PercussionOnEvent{0, "AcBass", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "AcBass", 64});
        track.addEvent(bw_music::PercussionOnEvent{0, "Cowbll", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Cowbll", 64});
        // LFlTom removed.
        // An unrecognized instrument.
        track.addEvent(bw_music::PercussionOnEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Dummy", 64});
        track.addEvent(bw_music::PercussionOnEvent{0, "Crash2", 64});
        track.addEvent(bw_music::PercussionOffEvent{babelwires::Rational(1, 2), "Crash2", 64});
        return track.finishAndGetTrack();
    }
} // namespace

TEST(PercussionMapProcessorTest, funcSimple) {
    testUtils::TestLog log;

    babelwires::TypeSystem typeSystem;
    const bw_music::BuiltInPercussionInstruments* const builtInPercussion =
        typeSystem.addEntry<bw_music::BuiltInPercussionInstruments>();
    typeSystem.addTypeConstructor<babelwires::EnumAtomTypeConstructor>();
    typeSystem.addTypeConstructor<babelwires::EnumUnionTypeConstructor>();

    const babelwires::MapValue mapValue = getTestPercussionMap(typeSystem);
    const bw_music::Track inputTrack = getTestInputTrack();
    const bw_music::Track outputTrack = bw_music::mapPercussionFunction(typeSystem, inputTrack, mapValue);
    const bw_music::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}

TEST(PercussionMapProcessorTest, processor) {
    testUtils::TestEnvironment testEnvironment;
    bw_music::registerLib(testEnvironment.m_projectContext);

    bw_music::PercussionMapProcessor processor(testEnvironment.m_projectContext);

    processor.getInput().setToDefault();
    processor.getOutput().setToDefault();

    babelwires::ValueTreeNode& input = processor.getInput();
    const babelwires::ValueTreeNode& output = processor.getOutput();

    babelwires::ValueTreeNode& inputArray =
        input.getChildFromStep(bw_music::PercussionMapProcessor::getCommonArrayId()).is<babelwires::ValueTreeNode>();
    const babelwires::ValueTreeNode& outputArray =
        output.getChildFromStep(bw_music::PercussionMapProcessor::getCommonArrayId()).is<babelwires::ValueTreeNode>();

    babelwires::ArrayInstanceImpl<babelwires::ValueTreeNode, bw_music::TrackType> inArray(inputArray);
    const babelwires::ArrayInstanceImpl<const babelwires::ValueTreeNode, bw_music::TrackType> outArray(outputArray);

    bw_music::PercussionMapProcessorInput::Instance in(input);

    EXPECT_EQ(inArray.getSize(), 1);
    EXPECT_EQ(outArray.getSize(), 1);

    EXPECT_EQ(inArray.getEntry(0).get().getDuration(), 0);
    EXPECT_EQ(outArray.getEntry(0).get().getDuration(), 0);
    processor.getOutput().setToDefault();

    in.getMap()->setValue(getTestPercussionMap(testEnvironment.m_typeSystem));
    inArray.getEntry(0).set(getTestInputTrack());

    processor.process(testEnvironment.m_log);

    const bw_music::Track outputTrack = outArray.getEntry(0).get();
    const bw_music::Track expectedOutputTrack = getTestOutputTrack();

    EXPECT_EQ(outputTrack, expectedOutputTrack);
}
