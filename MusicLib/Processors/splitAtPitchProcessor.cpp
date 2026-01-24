/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/splitAtPitchProcessor.hpp>

#include <MusicLib/Functions/splitAtPitchFunction.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

#include <BaseLib/Identifiers/registeredIdentifier.hpp>

bw_music::SplitAtPitchProcessorInput::SplitAtPitchProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("Pitch", "Pitch", "6b721baa-084f-450b-bf35-2e08a9592958"),
                               bw_music::PitchEnum::getThisIdentifier()},
                              {BW_SHORT_ID("Input", "Input Track", "9314a43f-256a-4915-b218-f2ba37133863"),
                               DefaultTrackType::getThisIdentifier()}}) {}

bw_music::SplitAtPitchProcessorOutput::SplitAtPitchProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {
          {BW_SHORT_ID("Above", "Notes at/above", "4c65b7f1-e546-4df2-9891-23623b74bc23"),
           DefaultTrackType::getThisIdentifier()},
          {BW_SHORT_ID("Below", "Notes below", "ab6a1d57-8c77-4df9-baf7-4b24136d9279"),
           DefaultTrackType::getThisIdentifier()},
          {BW_SHORT_ID("Other", "Other", "83bf663c-2931-467f-8403-b12e18138c68"),
           DefaultTrackType::getThisIdentifier()},
      }) {}

bw_music::SplitAtPitchProcessor::SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, SplitAtPitchProcessorInput::getThisIdentifier(),
                     SplitAtPitchProcessorOutput::getThisIdentifier()) {}

void bw_music::SplitAtPitchProcessor::processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input, babelwires::ValueTreeNode& output) const {
    SplitAtPitchProcessorInput::ConstInstance in{input};
    auto pitch = in.getPitch();
    auto trackIn = in.getInput();
    if (pitch->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged) ||
        trackIn->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        const int pitchIndex = pitch.getInstanceType().tryGetIndexFromIdentifier(pitch.get().get());
        if (pitchIndex >= 0) {
            auto newTracksOut = splitAtPitch(Pitch(pitchIndex), trackIn.get());
            SplitAtPitchProcessorOutput::Instance out{output};
            out.getAbove().set(std::move(newTracksOut.m_equalOrAbove));
            out.getBelow().set(std::move(newTracksOut.m_below));
            out.getOther().set(std::move(newTracksOut.m_other));
        } else {
            throw babelwires::ModelException() << "A valid pitch value was not specified in the SplitAtPitchProcessor";
        }
    }
}
