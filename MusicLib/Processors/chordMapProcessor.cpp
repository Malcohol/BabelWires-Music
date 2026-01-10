/**
 * Processor which applies a map to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/chordMapProcessor.hpp>

#include <MusicLib/Functions/mapChordsFunction.hpp>
#include <MusicLib/chord.hpp>
#include <MusicLib/pitch.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Map/mapTypeConstructor.hpp>
#include <BabelWiresLib/Types/Map/mapValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

bw_music::ChordMapProcessorInput::ChordMapProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(getThisType(), typeSystem,
          {{BW_SHORT_ID("ChrdMp", "Chord map", "6054b8e9-5f48-4e9f-8807-b6377d36d6aa"),
            babelwires::MapTypeConstructor::makeTypeExp(bw_music::getMapChordFunctionSourceTypeExp(),
                                                        bw_music::getMapChordFunctionTargetTypeExp(),
                                                        babelwires::MapEntryData::Kind::All21)}},
          ChordMapProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisType()) {}

bw_music::ChordMapProcessorOutput::ChordMapProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(getThisType(), typeSystem, ChordMapProcessor::getCommonArrayId(),
                                              bw_music::DefaultTrackType::getThisType()) {}

bw_music::ChordMapProcessor::ChordMapProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, ChordMapProcessorInput::getThisType(),
                                    ChordMapProcessorOutput::getThisType()) {}

babelwires::ShortId bw_music::ChordMapProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "24e56b0d-eb1e-4c93-97fd-ba4d639e112a");
}

void bw_music::ChordMapProcessor::processEntry(babelwires::UserLogger& userLogger,
                                               const babelwires::ValueTreeNode& input,
                                               const babelwires::ValueTreeNode& inputEntry,
                                               babelwires::ValueTreeNode& outputEntry) const {
    ChordMapProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    const auto& chordMap = in.getChrdMp()->getValue()->is<babelwires::MapValue>();

    entryOut.set(mapChordsFunction(in->getTypeSystem(), entryIn.get(), chordMap));
}
