/**
 * Processor which adjusts notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/fitToChordsProcessor.hpp>

#include <MusicLib/Functions/fitToChordFunction.hpp>


bw_music::FitToChordsProcessor::FitToChordsProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, FitToChordsProcessorInput::getThisType(),
                     FitToChordsProcessorOutput::getThisType()) {}



void bw_music::FitToChordsProcessor::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const {
    FitToChordsProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        FitToChordsProcessorOutput::Instance out{output};

        // TODO:
        // 1. Need to be able to query the assigned type of the input
        // 2. Need to be able to build record types dynamically.
    }
}
