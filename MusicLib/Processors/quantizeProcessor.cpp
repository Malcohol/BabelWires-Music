/**
 * A processor which quantizes sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/quantizeProcessor.hpp>

#include <MusicLib/Functions/quantizeFunction.hpp>

#include <BabelWiresLib/Types/Rational/rationalTypeConstructor.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

#include <set>

bw_music::QuantizeProcessorInput::QuantizeProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorInputBase(
          typeSystem,
          {{BW_SHORT_ID("Beat", "Beat", "1651ab49-3313-4cd3-b92d-16742b7f5921"),
            babelwires::RationalTypeConstructor::makeTypeExp(
                0, std::numeric_limits<babelwires::Rational::ComponentType>::max(), babelwires::Rational(1, 16))}},
          QuantizeProcessor::getCommonArrayId(), bw_music::DefaultTrackType::getThisType()) {}

bw_music::QuantizeProcessorOutput::QuantizeProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::ParallelProcessorOutputBase(typeSystem, QuantizeProcessor::getCommonArrayId(),
                                              bw_music::DefaultTrackType::getThisType()) {}

bw_music::QuantizeProcessor::QuantizeProcessor(const babelwires::ProjectContext& projectContext)
    : babelwires::ParallelProcessor(projectContext, QuantizeProcessorInput::getThisType(),
                                    QuantizeProcessorOutput::getThisType()) {}

babelwires::ShortId bw_music::QuantizeProcessor::getCommonArrayId() {
    return BW_SHORT_ID("Tracks", "Tracks", "e00623bf-c0f0-4fee-b6c4-4f65df896bf3");
}

void bw_music::QuantizeProcessor::processEntry(babelwires::UserLogger& userLogger,
                                               const babelwires::ValueTreeNode& input,
                                               const babelwires::ValueTreeNode& inputEntry,
                                               babelwires::ValueTreeNode& outputEntry) const {
    QuantizeProcessorInput::ConstInstance in{input};
    babelwires::ConstInstance<TrackType> entryIn{inputEntry};
    babelwires::Instance<TrackType> entryOut{outputEntry};

    entryOut.set(quantize(entryIn.get(), in.getBeat().get()));
}
