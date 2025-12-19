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
    context.m_typeSystem.addEntry<DefaultTrackType>();
    context.m_typeSystem.addEntry<ChordType>();
    context.m_typeSystem.addEntry<NoChord>();
    context.m_typeSystem.addEntry<PitchClass>();
    context.m_typeSystem.addEntry<PitchEnum>();
    context.m_typeSystem.addEntry<BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry<Duration>();
    context.m_typeSystem.addEntry<Tempo>();
    context.m_typeSystem.addEntry<ChordTypeSet>();

    context.m_typeSystem.addEntry<SplitAtPitchProcessorInput>();
    context.m_typeSystem.addEntry<SplitAtPitchProcessorOutput>();
    context.m_processorReg.addProcessor<SplitAtPitchProcessor>();

    context.m_typeSystem.addEntry<MonophonicSubtracksPolicyEnum>();
    context.m_typeSystem.addEntry<MonophonicSubtracksProcessorInput>();
    context.m_typeSystem.addEntry<MonophonicSubtracksProcessorOutput>();
    context.m_processorReg.addProcessor<MonophonicSubtracksProcessor>();

    context.m_typeSystem.addEntry<FingeredChordsSustainPolicyEnum>();
    context.m_typeSystem.addEntry<FingeredChordsProcessorInput>();
    context.m_typeSystem.addEntry<FingeredChordsProcessorOutput>();
    context.m_processorReg.addProcessor<FingeredChordsProcessor>();

    context.m_typeSystem.addEntry<GetChordTypesProcessorInput>();
    context.m_typeSystem.addEntry<GetChordTypesProcessorOutput>();
    context.m_processorReg.addProcessor<GetChordTypesProcessor>();

    context.m_typeSystem.addEntry<BuildAccompanimentProcessorInput>();
    context.m_typeSystem.addEntry<BuildAccompanimentProcessorOutput>();
    context.m_processorReg.addProcessor<BuildAccompanimentProcessor>();

    context.m_typeSystem.addEntry<MergeProcessorInput>();
    context.m_typeSystem.addEntry<MergeProcessorOutput>();
    context.m_processorReg.addProcessor<MergeProcessor>();

    context.m_typeSystem.addEntry<SilenceProcessorInput>();
    context.m_typeSystem.addEntry<SilenceProcessorOutput>();
    context.m_processorReg.addProcessor<SilenceProcessor>();

    context.m_typeSystem.addEntry<ConcatenateProcessorInput>();
    context.m_typeSystem.addEntry<ConcatenateProcessorOutput>();
    context.m_processorReg.addProcessor<ConcatenateProcessor>();

    context.m_typeSystem.addEntry<ExcerptProcessorInput>();
    context.m_typeSystem.addEntry<ExcerptProcessorOutput>();
    context.m_processorReg.addProcessor<ExcerptProcessor>();

    context.m_typeSystem.addEntry<RepeatProcessorInput>();
    context.m_typeSystem.addEntry<RepeatProcessorOutput>();
    context.m_processorReg.addProcessor<RepeatProcessor>();

    context.m_typeSystem.addEntry<TransposeProcessorInput>();
    context.m_typeSystem.addEntry<TransposeProcessorOutput>();
    context.m_processorReg.addProcessor<TransposeProcessor>();

    context.m_typeSystem.addEntry<QuantizeProcessorInput>();
    context.m_typeSystem.addEntry<QuantizeProcessorOutput>();
    context.m_processorReg.addProcessor<QuantizeProcessor>();

    context.m_typeSystem.addEntry<ChordMapProcessorInput>();
    context.m_typeSystem.addEntry<ChordMapProcessorOutput>();
    context.m_processorReg.addProcessor<ChordMapProcessor>();

    context.m_typeSystem.addEntry<PercussionMapProcessorInput>();
    context.m_typeSystem.addEntry<PercussionMapProcessorOutput>();
    context.m_processorReg.addProcessor<PercussionMapProcessor>();

    context.m_typeSystem.addTypeConstructor<TrackTypeConstructor>();
    context.m_typeSystem.addTypeConstructor<PercussionMapType>();
}
