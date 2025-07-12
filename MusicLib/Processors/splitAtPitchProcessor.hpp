/**
 * A processor which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>
#include <MusicLib/pitch.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music {
    class SplitAtPitchProcessorInput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("PitchSplitIn", "Split At Pitch Input", "f901af3a-c27b-449c-961a-8f43dee7d9a6", 1);

        SplitAtPitchProcessorInput();

        DECLARE_INSTANCE_BEGIN(SplitAtPitchProcessorInput)
        DECLARE_INSTANCE_FIELD(Pitch, bw_music::PitchEnum)
        DECLARE_INSTANCE_FIELD(Input, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class SplitAtPitchProcessorOutput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("PitchSplitOut", "Split At Pitch Output", "50f790e1-0ef3-4f34-ad14-5a6762772e19", 1);

        SplitAtPitchProcessorOutput();

        DECLARE_INSTANCE_BEGIN(SplitAtPitchProcessorOutput)
        DECLARE_INSTANCE_FIELD(Above, bw_music::TrackType)
        DECLARE_INSTANCE_FIELD(Below, bw_music::TrackType)
        DECLARE_INSTANCE_FIELD(Other, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class SplitAtPitchProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("SplitAtPitchProcessor", "Split At Pitch", "c7b79e31-98f4-4d20-b946-f55113eb3b75");

        SplitAtPitchProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };

} // namespace bw_music
