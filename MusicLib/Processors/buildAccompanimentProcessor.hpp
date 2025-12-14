/**
 * Processor that builds a record of chord-adjusted tracks from an input type and ChordTypeSet.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Generic/genericType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music {
    class BuildAccompanimentProcessorInput : public babelwires::GenericType {
      public:
        REGISTERED_TYPE("BuildAccompIn", "Build Accompaniment In", "d1f8b0c2-3f4e-4c5a-9b6e-7c8d9e0f1a2b", 1);

        BuildAccompanimentProcessorInput();

        // GenericTypes do not support the instance system yet, so working with these
        // objects is unpleasant.
        // TODO Work out how to use the instance system with Generic types.
        static babelwires::ShortId getIdOfChordsArray();
        static babelwires::ShortId getIdOfInput();
    };

    class BuildAccompanimentProcessorOutput : public babelwires::GenericType {
      public:
        REGISTERED_TYPE("BuildAccompOut", "Build Accompaniment Out", "e3f4b5c6-7d8e-9f0a-b1c2-d3e4f5a6b7c8", 1);

        BuildAccompanimentProcessorOutput();

        static babelwires::ShortId getIdOfResult();
    };

    /// Processor that builds a record of chord-adjusted tracks from an input type and ChordTypeSet.
    class BuildAccompanimentProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("BuildAccomp", "Build Accompaniment", "d604f85c-110d-4c11-aa99-3684cf12ab58");

        BuildAccompanimentProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
