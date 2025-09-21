/**
 * A processor which quantizes quantizes a track to a given beat.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/instance.hpp>

#include <BabelWiresLib/Processors/parallelProcessor.hpp>
#include <BabelWiresLib/Types/Rational/rationalType.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>


namespace bw_music {

    class QuantizeProcessorInput : public babelwires::ParallelProcessorInputBase {
      public:
        REGISTERED_TYPE("QuantTrcksIn", "Quantize In", "86a46b16-69a3-41bb-bbb3-19f8cb0a4e4d", 1);

        QuantizeProcessorInput();

        DECLARE_INSTANCE_BEGIN(QuantizeProcessorInput)
        DECLARE_INSTANCE_FIELD(Beat, babelwires::RationalType)
        // No need to mention the array here.
        DECLARE_INSTANCE_END()
    };

    class QuantizeProcessorOutput : public babelwires::ParallelProcessorOutputBase {
      public:
        REGISTERED_TYPE("QuantTrcksOut", "Quantize Out", "89feb462-63b1-473a-bc77-29540bda43f7", 1);

        QuantizeProcessorOutput();
    };

    /// A processor which quantizes the events in a track a specified number of times.
    class QuantizeProcessor : public babelwires::ParallelProcessor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("QuantizeTracks", "Quantize", "1ae89077-2cfb-4071-910c-2f5dcfc85b17");

        QuantizeProcessor(const babelwires::ProjectContext& projectContext);

        static babelwires::ShortId getCommonArrayId();

        void processEntry(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          const babelwires::ValueTreeNode& inputEntry, babelwires::ValueTreeNode& outputEntry) const override;
    };

} // namespace bw_music
