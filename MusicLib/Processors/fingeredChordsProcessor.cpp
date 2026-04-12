/**
 * Processor which interprets note events as defining chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Processors/fingeredChordsProcessor.hpp>

#include <BaseLib/Context/context.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <BaseLib/Result/resultDSL.hpp>

bw_music::FingeredChordsProcessorInput::FingeredChordsProcessorInput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("Policy", "Policy", "a1dd2ae0-e91e-40fe-af4a-c74f2c7d978a"),
                               bw_music::FingeredChordsSustainPolicyEnum::getThisIdentifier()},
                              {BW_SHORT_ID("Notes", "Notes", "f0ef98dd-6b1a-4a11-ac21-5492ec7ce038"),
                               DefaultTrackType::getThisIdentifier()}}) {}

bw_music::FingeredChordsProcessorOutput::FingeredChordsProcessorOutput(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisIdentifier(), typeSystem, {{BW_SHORT_ID("Chords", "Chords", "25154c6f-fe60-4d0a-b991-7cb7aa43409a"),
                               DefaultTrackType::getThisIdentifier()}}) {}


bw_music::FingeredChordsProcessor::FingeredChordsProcessor(const babelwires::Context& context)
    : Processor(context, context.get<babelwires::TypeSystem>().getRegisteredType<FingeredChordsProcessorInput>(),
                     context.get<babelwires::TypeSystem>().getRegisteredType<FingeredChordsProcessorOutput>()) {}

babelwires::Result bw_music::FingeredChordsProcessor::processValue(babelwires::UserLogger& userLogger,
                                                      const babelwires::ValueTreeNode& input,
                                                      babelwires::ValueTreeNode& output) const {
    FingeredChordsProcessorInput::ConstInstance in{input};
    if (in->isChanged(babelwires::ValueTreeNode::Changes::SomethingChanged)) {
        FingeredChordsProcessorOutput::Instance out{output};
        ASSIGN_OR_ERROR(auto track, fingeredChordsFunction(in.getNotes().get(), in.getPolicy().get()));
        out.getChords().set(std::move(track));
    }
    return {};
}
