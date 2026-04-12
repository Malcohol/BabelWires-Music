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
    context.getService<babelwires::TypeSystem>().addType<DefaultTrackType>();
    context.getService<babelwires::TypeSystem>().addType<ChordType>();
    context.getService<babelwires::TypeSystem>().addType<NoChord>();
    context.getService<babelwires::TypeSystem>().addType<PitchClass>();
    context.getService<babelwires::TypeSystem>().addType<PitchEnum>();
    context.getService<babelwires::TypeSystem>().addType<BuiltInPercussionInstruments>();
    context.getService<babelwires::TypeSystem>().addType<Duration>();
    context.getService<babelwires::TypeSystem>().addType<Tempo>();
    context.getService<babelwires::TypeSystem>().addType<ChordTypeSet>(context.getService<babelwires::TypeSystem>());

    context.getService<babelwires::TypeSystem>().addTypeConstructor<TrackTypeConstructor>();
    context.getService<babelwires::TypeSystem>().addTypeConstructor<PercussionMapType>();

    context.getService<babelwires::TypeSystem>().addType<SplitAtPitchProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<SplitAtPitchProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<SplitAtPitchProcessor>();

    context.getService<babelwires::TypeSystem>().addType<MonophonicSubtracksPolicyEnum>();
    context.getService<babelwires::TypeSystem>().addType<MonophonicSubtracksProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<MonophonicSubtracksProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<MonophonicSubtracksProcessor>();

    context.getService<babelwires::TypeSystem>().addType<FingeredChordsSustainPolicyEnum>();
    context.getService<babelwires::TypeSystem>().addType<FingeredChordsProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<FingeredChordsProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<FingeredChordsProcessor>();

    context.getService<babelwires::TypeSystem>().addType<GetChordTypesProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<GetChordTypesProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<GetChordTypesProcessor>();

    context.getService<babelwires::TypeSystem>().addType<BuildAccompanimentProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<BuildAccompanimentProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<BuildAccompanimentProcessor>();

    context.getService<babelwires::TypeSystem>().addType<MergeProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<MergeProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<MergeProcessor>();

    context.getService<babelwires::TypeSystem>().addType<SilenceProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<SilenceProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<SilenceProcessor>();

    context.getService<babelwires::TypeSystem>().addType<ConcatenateProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<ConcatenateProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<ConcatenateProcessor>();

    context.getService<babelwires::TypeSystem>().addType<ExcerptProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<ExcerptProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<ExcerptProcessor>();

    context.getService<babelwires::TypeSystem>().addType<RepeatProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<RepeatProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<RepeatProcessor>();

    context.getService<babelwires::TypeSystem>().addType<TransposeProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<TransposeProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<TransposeProcessor>();

    context.getService<babelwires::TypeSystem>().addType<QuantizeProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<QuantizeProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<QuantizeProcessor>();

    context.getService<babelwires::TypeSystem>().addType<ChordMapProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<ChordMapProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<ChordMapProcessor>();

    context.getService<babelwires::TypeSystem>().addType<PercussionMapProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<PercussionMapProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<PercussionMapProcessor>();

    context.getService<babelwires::TypeSystem>().addType<AccompanimentSequencerProcessorInput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<AccompanimentSequencerProcessorOutput>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::ProcessorFactoryRegistry>().addProcessor<AccompanimentSequencerProcessor>();
}
