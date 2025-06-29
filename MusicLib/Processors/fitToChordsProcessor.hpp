/**
 * Processor which adjusts notes in C major to fit other chord types.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Functions/fitToChordFunction.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>

namespace bw_music {

    class FitToChordsProcessorInput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("FitToChordIn", "Fit To Chord In", "d1f8b0c2-3f4e-4c5a-9b6e-7c8d9e0f1a2b", 1);

        FitToChordsProcessorInput();

        DECLARE_INSTANCE_BEGIN(FitToChordProcessorInput)
        DECLARE_INSTANCE_ARRAY_FIELD(Chord, Chord)
        DECLARE_INSTANCE_FIELD(Input, Type)
        DECLARE_INSTANCE_END()
    };

    class FitToChordsProcessorOutput : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("FitToChordOut", "Fit To Chord Out", "e3f4b5c6-7d8e-9f0a-b1c2-d3e4f5a6b7c8", 1);

        FitToChordsProcessorOutput();

        DECLARE_INSTANCE_BEGIN(FitToChordProcessorOutput)
        DECLARE_INSTANCE_FIELD(Output, RecordType)
        DECLARE_INSTANCE_END()
    };

    class FitToChordsProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("FitToChords", "Fit to Chords", "d604f85c-110d-4c11-aa99-3684cf12ab58");

        FitToChordsProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
