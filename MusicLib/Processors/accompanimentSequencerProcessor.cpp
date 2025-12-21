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
    if (!assignedInputTypeRef) {
        return;
    }

    const auto& chordTrack = inputChordTrack.getValue()->is<bw_music::Track>();
    const auto& accompanimentTracksRecordType = assignedInputTypeRef.resolve(typeSystem);

    const auto* accompanimentRecord = accompanimentTracksRecordType.as<babelwires::RecordType>();
    if (!accompanimentRecord) {
        throw babelwires::ModelException()
            << "Accompaniment tracks input is expected to be a record type with a field for each supported chord type";
    }
    if (input.isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        // Call the function
        const auto [resultType, resultValue] = accompanimentSequencerFunction(
            typeSystem, *accompanimentRecord, inputAccompanimentTracks.getValue(), chordTrack);

        if (!resultType) {
            return;
        }

        babelwires::ValueTreeNode& outputRecord = *output.getChild(0);
        babelwires::ValueTreeNode& outputResult = *outputRecord.getChild(0);
        babelwires::ValueHolder newOutputValue = output.getValue();

        outputType.setTypeVariableAssignmentAndInstantiate(typeSystem, newOutputValue, {resultType});

        // Set the output
        outputResult.setValue(resultValue);
        output.setValue(newOutputValue);
    }
}
