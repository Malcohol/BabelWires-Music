/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/getChordTypesProcessor.hpp>

#include <MusicLib/Types/duration.hpp>
#include <MusicLib/Functions/getChordTypesFunction.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

bw_music::GetChordTypesProcessorInput::GetChordTypesProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(typeSystem,
          {{BW_SHORT_ID("Track", "Track", "0acf2346-0e45-44ed-8a5d-4c6d0c2c1244"), DefaultTrackType::getThisType()}}) {}

bw_music::GetChordTypesProcessorOutput::GetChordTypesProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(typeSystem, {{BW_SHORT_ID("Chords", "Chords", "5f1aae17-1d3c-4a0c-9cbc-550cd2ff88a2"),
                               ChordTypeSet::getThisType()}}) {}

bw_music::GetChordTypesProcessor::GetChordTypesProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, GetChordTypesProcessorInput::getThisType(),
                     GetChordTypesProcessorOutput::getThisType()) {}

void bw_music::GetChordTypesProcessor::processValue(babelwires::UserLogger& userLogger,
                                              const babelwires::ValueTreeNode& input,
                                              babelwires::ValueTreeNode& output) const {
    GetChordTypesProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        GetChordTypesProcessorOutput::Instance out{output};
        const ChordTypeSet& chordTypeSetType = out.getChords()->getType()->is<ChordTypeSet>();
        auto setOfChordTypes = bw_music::getChordTypesFunction(in.getTrack().get());
        out.getChords()->setValue(chordTypeSetType.createValueFromChordTypes(in->getTypeSystem(), setOfChordTypes));
    }
}