/**
 * A processor which extracts a section of sequence data from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>


namespace bw_music {

    class MUSICLIB_API ConcatenateProcessorInput : public babelwires::RecordType {
      public:
        DOWNCASTABLE(ConcatenateProcessorInput, babelwires::RecordType);
        REGISTERED_TYPE("ConcatTrcksIn", "Concatenate In", "f4f21fe1-25e6-4721-a298-36fe27b532cc", 1);

        ConcatenateProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(ConcatenateProcessorInput)
        DECLARE_INSTANCE_ARRAY_FIELD(Input, TrackType)
        DECLARE_INSTANCE_END()
    };

    class MUSICLIB_API ConcatenateProcessorOutput : public babelwires::RecordType {
      public:
        DOWNCASTABLE(ConcatenateProcessorOutput, babelwires::RecordType);
        REGISTERED_TYPE("ConcatTrcksOut", "Concatenate Out", "2c9e13aa-eb88-494f-b3a6-7fb82504196c", 1);

        ConcatenateProcessorOutput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(ConcatenateProcessorOutput)
        DECLARE_INSTANCE_FIELD(Output, TrackType)
        DECLARE_INSTANCE_END()
    };

    class MUSICLIB_API ConcatenateProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("ConcatenateTracks", "Concatenate", "42b00d10-9d16-42d2-8ba6-971aad016da0");

        ConcatenateProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        babelwires::Result processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };
} // namespace bw_music
