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
#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

void bw_music::registerLib(babelwires::ProjectContext& context) {
    context.m_typeSystem.addType<DefaultTrackType>();
    context.m_typeSystem.addType<ChordType>();
    context.m_typeSystem.addType<NoChord>();
    context.m_typeSystem.addType<PitchClass>();
    context.m_typeSystem.addType<PitchEnum>();
    context.m_typeSystem.addType<BuiltInPercussionInstruments>();
    context.m_typeSystem.addType<Duration>();
    context.m_typeSystem.addType<Tempo>();
    context.m_typeSystem.addType<ChordTypeSet>(context.m_typeSystem);

    context.m_typeSystem.addTypeConstructor<TrackTypeConstructor>();
    context.m_typeSystem.addTypeConstructor<PercussionMapType>();

    context.m_typeSystem.addType<SplitAtPitchProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<SplitAtPitchProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<SplitAtPitchProcessor>();

    context.m_typeSystem.addType<MonophonicSubtracksPolicyEnum>();
    context.m_typeSystem.addType<MonophonicSubtracksProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<MonophonicSubtracksProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<MonophonicSubtracksProcessor>();

    context.m_typeSystem.addType<FingeredChordsSustainPolicyEnum>();
    context.m_typeSystem.addType<FingeredChordsProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<FingeredChordsProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<FingeredChordsProcessor>();

    context.m_typeSystem.addType<GetChordTypesProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<GetChordTypesProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<GetChordTypesProcessor>();

    context.m_typeSystem.addType<BuildAccompanimentProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<BuildAccompanimentProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<BuildAccompanimentProcessor>();

    context.m_typeSystem.addType<MergeProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<MergeProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<MergeProcessor>();

    context.m_typeSystem.addType<SilenceProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<SilenceProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<SilenceProcessor>();

    context.m_typeSystem.addType<ConcatenateProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<ConcatenateProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<ConcatenateProcessor>();

    context.m_typeSystem.addType<ExcerptProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<ExcerptProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<ExcerptProcessor>();

    context.m_typeSystem.addType<RepeatProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<RepeatProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<RepeatProcessor>();

    context.m_typeSystem.addType<TransposeProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<TransposeProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<TransposeProcessor>();

    context.m_typeSystem.addType<QuantizeProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<QuantizeProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<QuantizeProcessor>();

    context.m_typeSystem.addType<ChordMapProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<ChordMapProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<ChordMapProcessor>();

    context.m_typeSystem.addType<PercussionMapProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<PercussionMapProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<PercussionMapProcessor>();

    context.m_typeSystem.addType<AccompanimentSequencerProcessorInput>(context.m_typeSystem);
    context.m_typeSystem.addType<AccompanimentSequencerProcessorOutput>(context.m_typeSystem);
    context.m_processorReg.addProcessor<AccompanimentSequencerProcessor>();
}
