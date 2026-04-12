/**
 * Register factories etc. for BabelWires-Music into BabelWires.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/libRegistration.hpp>

#include <MusicLib/Functions/fingeredChordsFunction.hpp>
#include <MusicLib/Functions/monophonicSubtracksFunction.hpp>
#include <MusicLib/Functions/percussionMapFunction.hpp>
#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Processors/accompanimentSequencerProcessor.hpp>
#include <MusicLib/Processors/chordMapProcessor.hpp>
#include <MusicLib/Processors/concatenateProcessor.hpp>
#include <MusicLib/Processors/excerptProcessor.hpp>
#include <MusicLib/Processors/fingeredChordsProcessor.hpp>
#include <MusicLib/Processors/getChordTypesProcessor.hpp>
#include <MusicLib/Processors/buildAccompanimentProcessor.hpp>
#include <MusicLib/Processors/mergeProcessor.hpp>
#include <MusicLib/Processors/monophonicSubtracksProcessor.hpp>
#include <MusicLib/Processors/percussionMapProcessor.hpp>
#include <MusicLib/Processors/quantizeProcessor.hpp>
#include <MusicLib/Processors/repeatProcessor.hpp>
#include <MusicLib/Processors/silenceProcessor.hpp>
#include <MusicLib/Processors/splitAtPitchProcessor.hpp>
#include <MusicLib/Processors/transposeProcessor.hpp>
#include <MusicLib/Types/chordTypeSet.hpp>
#include <MusicLib/Types/Track/trackTypeConstructor.hpp>
#include <MusicLib/Types/tempo.hpp>
#include <MusicLib/Types/duration.hpp>
#include <MusicLib/chord.hpp>
#include <MusicLib/pitch.hpp>

#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processorFactoryRegistry.hpp>
#include <BaseLib/Context/context.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

void bw_music::registerLib(babelwires::Context& context) {
    babelwires::TypeSystem& typeSystem = context.get<babelwires::TypeSystem>();
    babelwires::ProcessorFactoryRegistry& processorFactoryRegistry = context.get<babelwires::ProcessorFactoryRegistry>();

    typeSystem.addType<DefaultTrackType>();
    typeSystem.addType<ChordType>();
    typeSystem.addType<NoChord>();
    typeSystem.addType<PitchClass>();
    typeSystem.addType<PitchEnum>();
    typeSystem.addType<BuiltInPercussionInstruments>();
    typeSystem.addType<Duration>();
    typeSystem.addType<Tempo>();
    typeSystem.addType<ChordTypeSet>(typeSystem);

    typeSystem.addTypeConstructor<TrackTypeConstructor>();
    typeSystem.addTypeConstructor<PercussionMapType>();

    typeSystem.addType<SplitAtPitchProcessorInput>(typeSystem);
    typeSystem.addType<SplitAtPitchProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<SplitAtPitchProcessor>();

    typeSystem.addType<MonophonicSubtracksPolicyEnum>();
    typeSystem.addType<MonophonicSubtracksProcessorInput>(typeSystem);
    typeSystem.addType<MonophonicSubtracksProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<MonophonicSubtracksProcessor>();

    typeSystem.addType<FingeredChordsSustainPolicyEnum>();
    typeSystem.addType<FingeredChordsProcessorInput>(typeSystem);
    typeSystem.addType<FingeredChordsProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<FingeredChordsProcessor>();

    typeSystem.addType<GetChordTypesProcessorInput>(typeSystem);
    typeSystem.addType<GetChordTypesProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<GetChordTypesProcessor>();

    typeSystem.addType<BuildAccompanimentProcessorInput>(typeSystem);
    typeSystem.addType<BuildAccompanimentProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<BuildAccompanimentProcessor>();

    typeSystem.addType<MergeProcessorInput>(typeSystem);
    typeSystem.addType<MergeProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<MergeProcessor>();

    typeSystem.addType<SilenceProcessorInput>(typeSystem);
    typeSystem.addType<SilenceProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<SilenceProcessor>();

    typeSystem.addType<ConcatenateProcessorInput>(typeSystem);
    typeSystem.addType<ConcatenateProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<ConcatenateProcessor>();

    typeSystem.addType<ExcerptProcessorInput>(typeSystem);
    typeSystem.addType<ExcerptProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<ExcerptProcessor>();

    typeSystem.addType<RepeatProcessorInput>(typeSystem);
    typeSystem.addType<RepeatProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<RepeatProcessor>();

    typeSystem.addType<TransposeProcessorInput>(typeSystem);
    typeSystem.addType<TransposeProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<TransposeProcessor>();

    typeSystem.addType<QuantizeProcessorInput>(typeSystem);
    typeSystem.addType<QuantizeProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<QuantizeProcessor>();

    typeSystem.addType<ChordMapProcessorInput>(typeSystem);
    typeSystem.addType<ChordMapProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<ChordMapProcessor>();

    typeSystem.addType<PercussionMapProcessorInput>(typeSystem);
    typeSystem.addType<PercussionMapProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<PercussionMapProcessor>();

    typeSystem.addType<AccompanimentSequencerProcessorInput>(typeSystem);
    typeSystem.addType<AccompanimentSequencerProcessorOutput>(typeSystem);
    processorFactoryRegistry.addProcessor<AccompanimentSequencerProcessor>();
}
