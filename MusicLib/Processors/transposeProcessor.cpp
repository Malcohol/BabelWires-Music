/**
 * A processor that adjusts the pitch of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/transposeProcessor.hpp>

#include <MusicLib/Functions/transposeFunction.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

bw_music::TransposeProcessorInput::TransposeProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(
          getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Offset", "Pitch Offset", "5cfa1541-f25e-4671-ac11-2ff71c883418"),
            babelwires::IntTypeConstructor::makeTypeExp(-127, 127, 0)}},
          TransposeProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisIdentifier()) {}

bw_music::TransposeProcessor::TransposeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, TransposeProcessorInput::getThisIdentifier(),
                                    TransposeProcessorOutput::getThisIdentifier()) {}

bw_music::TransposeProcessorOutput::TransposeProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(getThisIdentifier(), typeSystem, TransposeProcessor::getCommonArrayId(),
                                              bw_music::DefaultTrackType::getThisIdentifier()) {}

babelwires::ShortId bw_music::TransposeProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "83f05b66-7890-4542-8344-1409e50539b5");
}

void bw_music::TransposeProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                const babelwires::ValueTreeNode& input,
                                                const babelwires::ValueTreeNode& inputEntry,
                                                babelwires::ValueTreeNode& outputEntry) const {
    TransposeProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(transposeTrack(entryIn.get(), in.getOffset().get()));
}
