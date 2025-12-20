/**
 * A processor which sequences accompaniment tracks according to a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/accompanimentSequencerProcessor.hpp>

#include <MusicLib/Functions/accompanimentSequencerFunction.hpp>

#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>

bw_music::AccompanimentSequencerProcessorInput::AccompanimentSequencerProcessorInput()
    : babelwires::RecordType(
          {{getAccompTracksId(), babelwires::TypeVariableTypeConstructor::makeTypeRef()},
           {getChordTrackId(), DefaultTrackType::getThisType()}}) {}

bw_music::AccompanimentSequencerProcessorOutput::AccompanimentSequencerProcessorOutput()
    : babelwires::RecordType({{getResultId(), DefaultTrackType::getThisType()}}) {}

bw_music::AccompanimentSequencerProcessor::AccompanimentSequencerProcessor(
    const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, AccompanimentSequencerProcessorInput::getThisType(),
                AccompanimentSequencerProcessorOutput::getThisType()) {}

void bw_music::AccompanimentSequencerProcessor::processValue(babelwires::UserLogger& userLogger,
                                                             const babelwires::ValueTreeNode& input,
                                                             babelwires::ValueTreeNode& output) const {
    const AccompanimentSequencerProcessorInput& inputType = input.getType().is<AccompanimentSequencerProcessorInput>();

    const babelwires::ValueHolder& inputValue = input.getValue();
    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();

    /*
    // Get the accompaniment tracks field (field 0)
    const auto [accompanimentTracksValueHolder, accompanimentTracksStep, accompanimentTracksTypeRef] =
        inputType.getChild(inputValue, 0);
    const auto& accompanimentTracksRecordType =
        accompanimentTracksTypeRef.resolve(typeSystem).is<babelwires::RecordType>();

    // Get the chord track field (field 1)
    const auto [chordTrackValueHolder, chordTrackStep, chordTrackTypeRef] = inputType.getChild(inputValue, 1);
    const auto& chordTrack = (**chordTrackValueHolder).is<Track>();

    // Process only if something changed
    if (input.isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        // Call the function
        babelwires::ValueHolder resultValue = accompanimentSequencerFunction(
            typeSystem, accompanimentTracksRecordType, *accompanimentTracksValueHolder, chordTrack);

        // Set the output
        babelwires::ValueHolder outputValue = output.getValue();
        const AccompanimentSequencerProcessorOutput& outputType = output.getType().is<AccompanimentSequencerProcessorOutput>();
        auto [resultChild, resultStep, resultTypeRef] = outputType.getChildNonConst(outputValue, 0);
        *resultChild = std::move(resultValue);
        output.setValue(outputValue);
    }
        */
}

