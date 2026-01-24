/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/repeatProcessor.hpp>

#include <MusicLib/Functions/appendTrackFunction.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>

#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intValue.hpp>

#include <BaseLib/Identifiers/registeredIdentifier.hpp>

bw_music::RepeatProcessorInput::RepeatProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(
          getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"),
            babelwires::IntTypeConstructor::makeTypeExp(0, std::numeric_limits<babelwires::IntValue::NativeType>::max(),
                                                        2)}},
          RepeatProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisIdentifier()) {}

bw_music::RepeatProcessorOutput::RepeatProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(getThisIdentifier(), typeSystem, RepeatProcessor::getCommonArrayId(),
                                              bw_music::DefaultTrackType::getThisIdentifier()) {}

bw_music::RepeatProcessor::RepeatProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, RepeatProcessorInput::getThisIdentifier(),
                                    RepeatProcessorOutput::getThisIdentifier()) {}

babelwires::ShortId bw_music::RepeatProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c");
}

void bw_music::RepeatProcessor::processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                                             const babelwires::ValueTreeNode& inputEntry,
                                             babelwires::ValueTreeNode& outputEntry) const {
    RepeatProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};
    Track trackOut;

    const Track& trackIn = entryIn.get();
    for (int i = 0; i < in.getCount().get(); ++i) {
        appendTrack(trackOut, trackIn);
    }
    entryOut.set(std::move(trackOut));
}
