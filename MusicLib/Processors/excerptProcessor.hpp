/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/MusicLib/musicLibExport.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>

namespace bw_music {

    class MUSICLIB_API ExcerptProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        DOWNCASTABLE(ExcerptProcessorInput, babelwires::ParallelProcessorInputBase);
        REGISTERED_TYPE("ExcerptTrckIn", "Excerpt In", "68577705-b5c2-499c-a2db-8ab5f63e5728", 1);

        ExcerptProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(ExcerptProcessorInput)
        DECLARE_INSTANCE_FIELD(Start, babelwires::RationalType)
        DECLARE_INSTANCE_FIELD(Duratn, babelwires::RationalType)
        // No need to mention the array here.
        DECLARE_INSTANCE_END()
    };

    class MUSICLIB_API ExcerptProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        DOWNCASTABLE(ExcerptProcessorOutput, babelwires::ParallelProcessorOutputBase);
        REGISTERED_TYPE("ExcerptTrckOut", "Excerpt Out", "73469491-111c-441a-b89c-2f8aceaa640c", 1);

        ExcerptProcessorOutput(const babelwires::TypeSystem& typeSystem);
    };

    /// A processor which limits a track to events between certain points.
    class MUSICLIB_API ExcerptProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("TrackExcerpt", "Excerpt", "83c74dba-7861-447c-9abb-0b4439061baf");

        ExcerptProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        babelwires::Result processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
