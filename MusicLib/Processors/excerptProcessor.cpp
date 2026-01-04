/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/excerptProcessor.hpp>

#include <MusicLib/Functions/excerptFunction.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/duration.hpp>

#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

bw_music::ExcerptProcessorInput::ExcerptProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(typeSystem,
          {{BW_SHORT_ID("Start", "Start", "4b95f5db-a542-4660-a8db-97d3a5f831ca"), Duration::getThisType()},
           {BW_SHORT_ID("Duratn", "Duration", "d83ebbc2-1492-4578-a3b8-4969eb6a2042"), Duration::getThisType()}},
          ExcerptProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisType()) {}

bw_music::ExcerptProcessorOutput::ExcerptProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(typeSystem, ExcerptProcessor::getCommonArrayId(),
                                              bw_music::DefaultTrackType::getThisType()) {}

babelwires::ShortId bw_music::ExcerptProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "983b3bcb-7086-4791-8e18-d8c7550d45d3");
}

bw_music::ExcerptProcessor::ExcerptProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, ExcerptProcessorInput::getThisType(),
                                    ExcerptProcessorOutput::getThisType()) {}

void bw_music::ExcerptProcessor::processEntry(babelwires::UserLogger& userLogger,
                                              const babelwires::ValueTreeNode& input,
                                              const babelwires::ValueTreeNode& inputEntry,
                                              babelwires::ValueTreeNode& outputEntry) const {
    ExcerptProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(getTrackExcerpt(entryIn.get(), in.getStart().get(), in.getDuratn().get()));
}
