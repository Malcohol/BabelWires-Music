/**
 * A processor that adjusts the pitch of note events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>

namespace bw_music {

    class TransposeProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        REGISTERED_TYPE("TranspTrcksIn", "Transpose In", "980af793-e5fe-4fa4-861c-90f400fc4977", 1);

        TransposeProcessorInput();

        DECLARE_INSTANCE_BEGIN(TransposeProcessorInput)
        // TODO Expose an enum for TransposeOutOfRangePolicy (optional?)
        DECLARE_INSTANCE_FIELD(Offset, babelwires::IntType)
        DECLARE_INSTANCE_END()
    };

    class TransposeProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        REGISTERED_TYPE("TranspTrcksOut", "Transpose Out", "e2c9d64e-259a-4ef5-bd9d-0883c5d67599", 1);

        TransposeProcessorOutput();
    };


    /// A processor that adjusts the pitch of note events.
    class TransposeProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("TransposeTracks", "Transpose", "3414f6cf-290a-421e-bce5-6a98ed0483af");

        TransposeProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
