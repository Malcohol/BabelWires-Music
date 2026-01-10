/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/percussionMapProcessor.hpp>

#include <MusicLib/Functions/percussionMapFunction.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

bw_music::PercussionMapProcessorInput::PercussionMapProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(getThisType(), typeSystem, 
          {{BW_SHORT_ID("Map", "Map", "b8cbf8c9-579b-4292-bdef-524b7d1010bc"), bw_music::getPercussionMapType()}},
          PercussionMapProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisType()) {}

bw_music::PercussionMapProcessorOutput::PercussionMapProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(getThisType(), typeSystem, PercussionMapProcessor::getCommonArrayId(),
                                                   bw_music::DefaultTrackType::getThisType()) {}

bw_music::PercussionMapProcessor::PercussionMapProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, PercussionMapProcessorInput::getThisType(),
                                         PercussionMapProcessorOutput::getThisType()) {}

babelwires::ShortId bw_music::PercussionMapProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "fe71b1c6-6604-430b-a731-f40b2692d2cf");
}

void bw_music::PercussionMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                                    const babelwires::ValueTreeNode& input,
                                                    const babelwires::ValueTreeNode& inputEntry,
                                                    babelwires::ValueTreeNode& outputEntry) const {
    PercussionMapProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    const auto& percMap = in.getMap()->getValue()->is<babelwires::MapValue>();

    entryOut.set(mapPercussionFunction(in->getTypeSystem(), entryIn.get(), percMap));
}
