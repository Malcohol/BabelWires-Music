/**
 * A processor which sequences accompaniment tracks according to a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/accompanimentSequencerProcessor.hpp>

#include <MusicLib/Functions/accompanimentSequencerFunction.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Generic/typeVariableTypeConstructor.hpp>
#include <BabelWiresLib/Types/Record/recordTypeConstructor.hpp>
#include <BabelWiresLib/ValueTree/modelExceptions.hpp>
#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>
#include <BabelWiresLib/Types/Failure/failureType.hpp>

bw_music::AccompanimentSequencerProcessorInput::AccompanimentSequencerProcessorInput()
    : babelwires::GenericType(babelwires::RecordTypeConstructor::makeTypeRef(
                                  getChordTrackId(), DefaultTrackType::getThisType(), getAccompTracksId(),
                                  babelwires::TypeVariableTypeConstructor::makeTypeRef()),
                              1) {}

bw_music::AccompanimentSequencerProcessorOutput::AccompanimentSequencerProcessorOutput()
    : babelwires::GenericType(babelwires::RecordTypeConstructor::makeTypeRef(
                                  getResultId(), babelwires::TypeVariableTypeConstructor::makeTypeRef()),
                              1) {}

bw_music::AccompanimentSequencerProcessor::AccompanimentSequencerProcessor(
    const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, AccompanimentSequencerProcessorInput::getThisType(),
                AccompanimentSequencerProcessorOutput::getThisType()) {}

void bw_music::AccompanimentSequencerProcessor::processValue(babelwires::UserLogger& userLogger,
                                                             const babelwires::ValueTreeNode& input,
                                                             babelwires::ValueTreeNode& output) const {
    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();
    const AccompanimentSequencerProcessorInput& inputType = input.getType().is<AccompanimentSequencerProcessorInput>();
    const AccompanimentSequencerProcessorOutput& outputType =
        output.getType().is<AccompanimentSequencerProcessorOutput>();

    const babelwires::ValueTreeNode& inputRecord = *input.getChild(0);
    const babelwires::ValueTreeNode& inputChordTrack = *inputRecord.getChild(0);
    const babelwires::ValueTreeNode& inputAccompanimentTracks = *inputRecord.getChild(1);

    const babelwires::ValueHolder& inputValue = input.getValue();
    const babelwires::TypeRef& assignedInputTypeRef = inputType.getTypeAssignment(inputValue, 0);

    if (input.isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        babelwires::ValueTreeNode& outputRecord = *output.getChild(0);
        babelwires::ValueTreeNode& outputResult = *outputRecord.getChild(0);
        babelwires::ValueHolder newOutputValue = output.getValue();

        if (!assignedInputTypeRef) {
            if (outputType.getTypeAssignment(newOutputValue, 0)) {
                outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {babelwires::TypeRef()});
                output.setValue(newOutputValue);
            }
            return;
        }

        const auto& accompanimentTracksRecordType = assignedInputTypeRef.resolve(typeSystem);

        const auto& typeForChords = getAccompanimentTypeForChords(typeSystem, accompanimentTracksRecordType);
        if (!typeForChords) {
            if (outputType.getTypeAssignment(newOutputValue, 0) != babelwires::FailureType::getThisType()) {
                outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {babelwires::FailureType::getThisType()});
                output.setValue(newOutputValue);
            }
            throw babelwires::ModelException()
                << "Accompaniment tracks input must have at least one field whose identifier is a chord type";
        } else {
            // TODO: Probably I shouldn't do this, but the build accompaniment function produces decorated types and it looks ugly.
            const auto undecoratedType = typeForChords.resolve(typeSystem).getTypeRef();
            outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {undecoratedType});
            output.setValue(newOutputValue);
        }

        const auto& chordTrack = inputChordTrack.getValue()->is<bw_music::Track>();

        // Call the function
        const auto resultValue = accompanimentSequencerFunction(
            typeSystem, accompanimentTracksRecordType, inputAccompanimentTracks.getValue(), chordTrack);

        // Set the output
        outputResult.setValue(resultValue);
    }
}

void bw_music::AccompanimentSequencerProcessor::onFailure() const {}