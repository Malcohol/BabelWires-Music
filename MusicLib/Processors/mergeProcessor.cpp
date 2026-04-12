/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/mergeProcessor.hpp>

#include <MusicLib/Functions/mergeFunction.hpp>

#include <BaseLib/Context/context.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>

#include <BaseLib/Identifiers/registeredIdentifier.hpp>
#include <BaseLib/Result/resultDSL.hpp>

#include <set>

bw_music::MergeProcessorInput::MergeProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Input", "Input tracks", "80b175ae-c954-4943-96d8-eaffcd7ed6e1"),
            babelwires::ArrayTypeConstructor::makeTypeExp(DefaultTrackType::getThisIdentifier(), 2, 16)}}) {}

bw_music::MergeProcessorOutput::MergeProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("Output", "Output track", "ab56e996-d361-42ed-a0df-44a90a73dc20"),
                               DefaultTrackType::getThisIdentifier()}}) {}

bw_music::MergeProcessor::MergeProcessor(const babelwires::Context& context)
    : Processor(context, context.getService<babelwires::TypeSystem>().getRegisteredType<MergeProcessorInput>(), context.getService<babelwires::TypeSystem>().getRegisteredType<MergeProcessorOutput>()) {}

babelwires::Result bw_music::MergeProcessor::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                                            babelwires::ValueTreeNode& output) const {
    MergeProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        std::vector<const Track*> tracksIn;
        for (int i = 0; i < in.getInput().getSize(); ++i) {
            tracksIn.emplace_back(&in.getInput().getEntry(i).get());
        }
        MergeProcessorOutput::Instance out{output};
        ASSIGN_OR_ERROR(auto track, mergeTracks(tracksIn));
        out.getOutput().set(std::move(track));
    }
    return {};
}
