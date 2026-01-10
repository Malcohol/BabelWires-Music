/**
 * A processor which creates a silent track of a certain duration.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/Types/chordTypeSet.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music {
    class GetChordTypesProcessorInput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("ChordTypesIn", "Get Chord Types In", "d74143a4-92c1-46a0-be85-350720a2045f", 1);

        GetChordTypesProcessorInput(const babelwires::TypeSystem& typeSystem);
        DECLARE_INSTANCE_BEGIN(GetChordTypesProcessorInput)
        DECLARE_INSTANCE_FIELD(Track, TrackType)
        DECLARE_INSTANCE_END()
    };

    class GetChordTypesProcessorOutput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("ChordTypesOut", "Get Chord Types Out", "6336361f-1861-4cda-8363-cdfc9f0950ed", 1);

        GetChordTypesProcessorOutput(const babelwires::TypeSystem& typeSystem);
        DECLARE_INSTANCE_BEGIN(GetChordTypesProcessorOutput)
        DECLARE_INSTANCE_NON_INSTANCE_FIELD(Chords)
        DECLARE_INSTANCE_END()
    };

    class GetChordTypesProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("GetChordTypes", "Get Chord Types", "20ee9695-8277-4a04-b380-098b0fd8c525");

        GetChordTypesProcessor(const babelwires::ProjectContext& projectContext);
      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
