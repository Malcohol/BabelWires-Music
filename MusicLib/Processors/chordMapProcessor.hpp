/**
 * Processor which applies a map to chord events.
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

    class ChordMapProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        REGISTERED_TYPE("ChordMapIn", "ChordMap In", "9a6aac86-fc46-40e2-91ba-c0fb053ad172", 1);

        ChordMapProcessorInput(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(ChordMapProcessorInput)
        DECLARE_INSTANCE_MAP_FIELD(ChrdMp, babelwires::EnumType, babelwires::EnumType)
        DECLARE_INSTANCE_END()
    };

    class ChordMapProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        REGISTERED_TYPE("ChordMapOut", "ChordMap Out", "e7ed549d-d6ef-4cca-b66e-5b271d00e0b2", 1);

        ChordMapProcessorOutput(const babelwires::TypeSystem& typeSystem);
    };

    /// A processor which chordmaps the events in a track a specified number of times.
    class ChordMapProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("ChordMapProcessor", "Chord Map", "b7227130-8274-4451-bd60-8fe34a74c4b6");

        ChordMapProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
