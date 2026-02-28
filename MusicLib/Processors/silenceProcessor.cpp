/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/silenceProcessor.hpp>

#include <MusicLib/Types/duration.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/Rational/rationalValue.hpp>

bw_music::SilenceProcessorInput::SilenceProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem,
          {{BW_SHORT_ID("Durn", "Duration", "05d3ea91-cb90-42f5-9988-2fb2e02e231c"), Duration::getThisIdentifier()}}) {}

bw_music::SilenceProcessorOutput::SilenceProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("Track", "Track", "86f3d028-a616-4a95-a566-a010ffcabb19"),
                               DefaultTrackType::getThisIdentifier()}}) {}

bw_music::SilenceProcessor::SilenceProcessor(const babelwires::ProjectContext& projectContext)
    : Processor(projectContext, projectContext.m_typeSystem.getRegisteredType<SilenceProcessorInput>(),
                     projectContext.m_typeSystem.getRegisteredType<SilenceProcessorOutput>()) {}

void bw_music::SilenceProcessor::processValue(babelwires::UserLogger& userLogger,
                                              const babelwires::ValueTreeNode& input,
                                              babelwires::ValueTreeNode& output) const {
    SilenceProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        SilenceProcessorOutput::Instance out{output};
        out.getTrack().set(Track(in.getDurn().get()));
    }
}