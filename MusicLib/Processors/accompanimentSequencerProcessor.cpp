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
#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>

#include <BaseLib/Context/context.hpp>

#include <BaseLib/Result/resultDSL.hpp>

bw_music::AccompanimentSequencerProcessorInput::AccompanimentSequencerProcessorInput(
    const babelwires::TypeSystem& typeSystem)
    : babelwires::GenericType(getThisIdentifier(), 
          typeSystem,
          babelwires::RecordTypeConstructor::makeTypeExp(getChordTrackId(), DefaultTrackType::getThisIdentifier(),
                                                         getAccompTracksId(), getGenericAccompanimentTypeExp()),
          1) {}

bw_music::AccompanimentSequencerProcessorOutput::AccompanimentSequencerProcessorOutput(
    const babelwires::TypeSystem& typeSystem)
    : babelwires::GenericType(getThisIdentifier(), typeSystem,
                              babelwires::RecordTypeConstructor::makeTypeExp(
                                  getResultId(), babelwires::TypeVariableTypeConstructor::makeTypeExp()),
                              1) {}

bw_music::AccompanimentSequencerProcessor::AccompanimentSequencerProcessor(
    const babelwires::Context& context)
    : Processor(context, context.getService<babelwires::TypeSystem>().getRegisteredType<AccompanimentSequencerProcessorInput>(),
                context.getService<babelwires::TypeSystem>().getRegisteredType<AccompanimentSequencerProcessorOutput>()) {}

babelwires::Result bw_music::AccompanimentSequencerProcessor::processValue(babelwires::UserLogger& userLogger,
                                                             const babelwires::ValueTreeNode& input,
                                                             babelwires::ValueTreeNode& output) const {
    const babelwires::TypeSystem& typeSystem = input.getTypeSystem();
    const AccompanimentSequencerProcessorInput& inputType = input.getType()->as<AccompanimentSequencerProcessorInput>();
    const AccompanimentSequencerProcessorOutput& outputType =
        output.getType()->as<AccompanimentSequencerProcessorOutput>();

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
        output.assertSetValue(newOutputValue);

        const auto& chordTrack = inputChordTrack.getValue()->as<bw_music::Track>();

        if (assignedInputTypeExp) {
            ASSIGN_OR_ERROR(auto resultValue, accompanimentSequencerFunction(typeSystem, *inputAccompanimentTracks.getType(),
                                                                    inputAccompanimentTracks.getValue(), chordTrack));
            outputResult.assertSetValue(std::move(resultValue));
        }
    }
    return {};
}

void bw_music::AccompanimentSequencerProcessor::onFailure() const {}