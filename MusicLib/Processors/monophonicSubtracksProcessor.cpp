/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/monophonicSubtracksProcessor.hpp>

#include <BabelWiresLib/Types/Array/arrayTypeConstructor.hpp>
#include <BabelWiresLib/Types/Int/intTypeConstructor.hpp>

bw_music::MonophonicSubtracksProcessorInput::MonophonicSubtracksProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("NumTrk", "Num subtracks", "30bc74d2-b678-4986-8296-929db40fc8c2"),
                               babelwires::IntTypeConstructor::makeTypeExp(1, 16, 1)},
                              {BW_SHORT_ID("Policy", "Policy", "c3192ee7-adec-4239-83a1-ef2d130ce421"),
                               MonophonicSubtracksPolicyEnum::getThisIdentifier()},
                              {BW_SHORT_ID("Input", "Input Track", "16e6745d-2456-489f-b73b-8704a442591b"),
                               DefaultTrackType::getThisIdentifier()}}) {}

bw_music::MonophonicSubtracksProcessorOutput::MonophonicSubtracksProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Sbtrks", "Mono tracks", "27c5fbe2-1060-4dc4-b46a-735b48128e17"),
            babelwires::ArrayTypeConstructor::makeTypeExp(DefaultTrackType::getThisIdentifier(), 0, 16)},
           {BW_SHORT_ID("Other", "Other", "bc3a5261-630c-43d7-bda5-f85dd6a1fe2b"), DefaultTrackType::getThisIdentifier()}}) {}

bw_music::MonophonicSubtracksProcessor::MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, MonophonicSubtracksProcessorInput::getThisIdentifier(),
                MonophonicSubtracksProcessorOutput::getThisIdentifier()) {}

void bw_music::MonophonicSubtracksProcessor::processValue(babelwires::UserLogger& userLogger,
                                                          const babelwires::ValueTreeNode& input,
                                                          babelwires::ValueTreeNode& output) const {
    MonophonicSubtracksProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        const unsigned int numTracks = in.getNumTrk().get();
        const MonophonicSubtracksPolicyEnum::Value policy = in.getPolicy().get();
        const bw_music::Track& trackIn = in.getInput().get();
        auto result = getMonophonicSubtracks(trackIn, numTracks, policy);

        MonophonicSubtracksProcessorOutput::Instance out{output};
        auto tracksOut = out.getSbtrks();
        tracksOut.setSize(numTracks);
        for (int i = 0; i < result.m_noteTracks.size(); ++i) {
            tracksOut.getEntry(i).set(std::move(result.m_noteTracks[i]));
        }
        out.getOther().set(std::move(result.m_other));
    }
}
