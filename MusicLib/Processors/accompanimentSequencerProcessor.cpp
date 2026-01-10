/**
 * A processor which sequences accompaniment tracks according to a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/accompanimentSequencerProcessor.hpp>

#include <MusicLib/Functions/accompanimentSequencerFunction.hpp>
#include <MusicLib/Types/genericAccompaniment.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Failure/failureType.hpp>
#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/Types/Record/recordTypeConstructor.hpp>
#include <BabelWiresLib/ValueTree/modelExceptions.hpp>
#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>

bw_music::AccompanimentSequencerProcessorInput::AccompanimentSequencerProcessorInput(
    const babelwires::TypeSystem& typeSystem)
    : babelwires::GenericType(getThisType(), 
          typeSystem,
          babelwires::RecordTypeConstructor::makeTypeExp(getChordTrackId(), DefaultTrackType::getThisType(),
                                                         getAccompTracksId(), getGenericAccompanimentTypeExp()),
          1) {}

bw_music::AccompanimentSequencerProcessorOutput::AccompanimentSequencerProcessorOutput(
    const babelwires::TypeSystem& typeSystem)
    : babelwires::GenericType(getThisType(), typeSystem,
                              babelwires::RecordTypeConstructor::makeTypeExp(
                                  getResultId(), babelwires::TypeVariableTypeConstructor::makeTypeExp()),
                              1) {}

bw_music::AccompanimentSequencerProcessor::AccompanimentSequencerProcessor(
    const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, AccompanimentSequencerProcessorInput::getThisType(),
                AccompanimentSequencerProcessorOutput::getThisType()) {}

void bw_music::AccompanimentSequencerProcessor::processValue(babelwires::UserLogger& userLogger,
                                                             const babelwires::ValueTreeNode& input,
                                                             babelwires::ValueTreeNode& output) const {
    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();
    const AccompanimentSequencerProcessorInput& inputType = input.getType()->is<AccompanimentSequencerProcessorInput>();
    const AccompanimentSequencerProcessorOutput& outputType =
        output.getType()->is<AccompanimentSequencerProcessorOutput>();

    const babelwires::ValueTreeNode& inputRecord = *input.getChild(0);
    const babelwires::ValueTreeNode& inputChordTrack = *inputRecord.getChild(0);
    const babelwires::ValueTreeNode& inputAccompanimentTracks = *inputRecord.getChild(1);

    const babelwires::ValueHolder& inputValue = input.getValue();
    const babelwires::TypeExp& assignedInputTypeExp = inputType.getTypeAssignment(inputValue, 0);

    if (input.isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        babelwires::ValueTreeNode& outputRecord = *output.getChild(0);
        babelwires::ValueTreeNode& outputResult = *outputRecord.getChild(0);
        babelwires::ValueHolder newOutputValue = output.getValue();

        const babelwires::TypeExp& assignedInputTypeExp = inputType.getTypeAssignment(inputValue, 0);
        outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {assignedInputTypeExp});
        output.setValue(newOutputValue);

        const auto& chordTrack = inputChordTrack.getValue()->is<bw_music::Track>();

        if (assignedInputTypeExp) {
            const auto resultValue = accompanimentSequencerFunction(typeSystem, *inputAccompanimentTracks.getType(),
                                                                    inputAccompanimentTracks.getValue(), chordTrack);
            outputResult.setValue(std::move(resultValue));
        }
    }
}

void bw_music::AccompanimentSequencerProcessor::onFailure() const {}