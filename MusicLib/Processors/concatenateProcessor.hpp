/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>


namespace bw_music {

    class ConcatenateProcessorInput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("ConcatTrcksIn", "Concatenate In", "f4f21fe1-25e6-4721-a298-36fe27b532cc", 1);

        ConcatenateProcessorInput();

        DECLARE_INSTANCE_BEGIN(ConcatenateProcessorInput)
        DECLARE_INSTANCE_ARRAY_FIELD(Input, TrackType)
        DECLARE_INSTANCE_END()
    };

    class ConcatenateProcessorOutput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("ConcatTrcksOut", "Concatenate Out", "2c9e13aa-eb88-494f-b3a6-7fb82504196c", 1);

        ConcatenateProcessorOutput();

        DECLARE_INSTANCE_BEGIN(ConcatenateProcessorOutput)
        DECLARE_INSTANCE_FIELD(Output, TrackType)
        DECLARE_INSTANCE_END()
    };

    class ConcatenateProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("ConcatenateTracks", "Concatenate", "42b00d10-9d16-42d2-8ba6-971aad016da0");

        ConcatenateProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };
} // namespace bw_music
