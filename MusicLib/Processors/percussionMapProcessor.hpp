/**
 * Processor which maps percussion events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/instance.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Types/Rational/rationalType.hpp>
#include <BabelWiresLib/Types/Map/mapType.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>

namespace bw_music {

    class PercussionMapProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        REGISTERED_TYPE("PercMapIn", "PercussionMap In", "d422110c-3ea4-42a3-86b0-311332836504", 1);

        PercussionMapProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(PercussionMapProcessorInput)
        DECLARE_INSTANCE_NON_INSTANCE_FIELD(Map)
        DECLARE_INSTANCE_END()
    };

    class PercussionMapProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        REGISTERED_TYPE("PercMapOut", "PercussionMap Out", "e0940d22-c79a-4139-a0ef-00aee485ef2a", 1);

        PercussionMapProcessorOutput(const babelwires::TypeSystem& typeSystem);
    };

    /// A processor which percussionmaps the events in a track a specified number of times.
    class PercussionMapProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("PercussionMapProcessor", "Percussion Map", "1ab6fd2b-8176-4516-9d9a-3b2d91a53f42");

        PercussionMapProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
