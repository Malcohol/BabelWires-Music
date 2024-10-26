/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <SeqWiresLib/Types/Track/trackInstance.hpp>
#include <SeqWiresLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/primitiveType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace seqwires {
    class MergeProcessorInput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("MergeTracksIn", "Merge Tracks Input", "15dd4564-e67f-4087-8609-ef5985b23dd7", 1);

        MergeProcessorInput();

        DECLARE_INSTANCE_BEGIN(MergeProcessorInput)
        DECLARE_INSTANCE_ARRAY_FIELD(Input, TrackType)
        DECLARE_INSTANCE_END()
    };

    class MergeProcessorOutput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("MergeTracksOut", "Merge Tracks Output", "9b797596-f6c2-4900-98a4-001ec7c18be4", 1);

        MergeProcessorOutput();

        DECLARE_INSTANCE_BEGIN(MergeProcessorOutput)
        DECLARE_INSTANCE_FIELD(Output, TrackType)
        DECLARE_INSTANCE_END()
    };

    class MergeProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("MergeTracks", "Merge", "ed004257-0ae3-44aa-abb9-d752c2eba0c1");

        MergeProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::Feature& inputFeature,
                          babelwires::Feature& outputFeature) const override;
    };

} // namespace seqwires
