/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Functions/monophonicSubtracksFunction.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music {
    class MonophonicSubtracksProcessorInput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("MonoTracksIn", "Monophonic Tracks Input", "e8041fe6-29de-470e-9235-e1f05c5f791e", 1);

        MonophonicSubtracksProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(MonophonicSubtracksProcessorInput)
        DECLARE_INSTANCE_FIELD(NumTrk, babelwires::IntType)
        DECLARE_INSTANCE_FIELD(Policy, MonophonicSubtracksPolicyEnum)
        DECLARE_INSTANCE_FIELD(Input, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class MonophonicSubtracksProcessorOutput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("MonoTracksOut", "Monophonic Tracks Output", "c2228921-da8a-45c0-bebf-24951d651090", 1);

        MonophonicSubtracksProcessorOutput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(MonophonicSubtracksProcessorOutput)
        DECLARE_INSTANCE_ARRAY_FIELD(Sbtrks, bw_music::TrackType)
        DECLARE_INSTANCE_FIELD(Other, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class MonophonicSubtracksProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("MonoSubtracksProcessor", "Monophonic subtracks", "7b6bbc49-24a5-4657-86fd-c457d77feaf9");

        MonophonicSubtracksProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
