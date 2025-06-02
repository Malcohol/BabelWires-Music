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

#include <Common/Identifiers/registeredIdentifier.hpp>

seqwires::RepeatProcessorInput::RepeatProcessorInput()
    : babelwires::ParallelProcessorInputBase(
          {{BW_SHORT_ID("Count", "Count", "f5d2ab08-4430-47fa-b26c-0ff2154826e3"),
            babelwires::IntTypeConstructor::makeTypeRef(0, std::numeric_limits<babelwires::IntValue::NativeType>::max(),
                                                        2)}},
          RepeatProcessor::getCommonArrayId(), seqwires::DefaultTrackType::getThisType()) {}

seqwires::RepeatProcessorOutput::RepeatProcessorOutput()
    : babelwires::ParallelProcessorOutputBase(RepeatProcessor::getCommonArrayId(),
                                              seqwires::DefaultTrackType::getThisType()) {}

seqwires::RepeatProcessor::RepeatProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, RepeatProcessorInput::getThisType(),
                                    RepeatProcessorOutput::getThisType()) {}

babelwires::ShortId seqwires::RepeatProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "f727937f-0215-4527-bab4-0eca269d6c5c");
}

void seqwires::RepeatProcessor::processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
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
