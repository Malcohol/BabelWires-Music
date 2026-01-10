/**
 * A processor which repeats sequence data a number of times.
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
    class RepeatProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        REGISTERED_TYPE("RepeatTrcksIn", "Repeat In", "23dc427d-8171-4de4-a9b6-15c16d9ed373", 1);

        RepeatProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(RepeatProcessorInput)
        DECLARE_INSTANCE_FIELD(Count, babelwires::IntType)
        // No need to mention the array here.
        DECLARE_INSTANCE_END()
    };

    class RepeatProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        REGISTERED_TYPE("RepeatTrcksOut", "Repeat Out", "c0d3c991-8e57-4bb9-86bd-68fdbf854434", 1);

        RepeatProcessorOutput(const babelwires::TypeSystem& typeSystem);
    };

    /// A processor which repeats the events in a track a specified number of times.
    class RepeatProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("RepeatTracks", "Repeat", "6c5b3e89-bb57-4c90-8a66-1d8cdeb29db9");

        RepeatProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
