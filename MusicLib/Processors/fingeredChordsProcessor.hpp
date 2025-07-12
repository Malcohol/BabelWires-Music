/**
 * Processor which interprets note events as defining chords.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/fingeredChordsFunction.hpp>
#include <MusicLib/Types/Track/trackInstance.hpp>
#include <MusicLib/Types/Track/trackType.hpp>

#include <BabelWiresLib/Instance/instance.hpp>
#include <BabelWiresLib/Processors/processorFactory.hpp>
#include <BabelWiresLib/Processors/processor.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Record/recordType.hpp>

namespace bw_music {
    class FingeredChordsProcessorInput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("FingrChordsIn", "Fingered Chords Input", "c9f26551-f557-4b27-ba0c-385298a2e51e", 1);

        FingeredChordsProcessorInput();

        DECLARE_INSTANCE_BEGIN(FingeredChordsProcessorInput)
        DECLARE_INSTANCE_FIELD(Policy, FingeredChordsSustainPolicyEnum)
        DECLARE_INSTANCE_FIELD(Notes, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class FingeredChordsProcessorOutput : public babelwires::RecordType {
      public:
        REGISTERED_TYPE("FingrChordsOut", "Fingered Chords Output", "f53cb215-1f8e-491b-8baf-868a5a5d0173", 1);

        FingeredChordsProcessorOutput();

        DECLARE_INSTANCE_BEGIN(FingeredChordsProcessorOutput)
        DECLARE_INSTANCE_FIELD(Chords, bw_music::TrackType)
        DECLARE_INSTANCE_END()
    };

    class FingeredChordsProcessor : public babelwires::Processor {
      public:
        BW_PROCESSOR_WITH_DEFAULT_FACTORY("FingeredChords", "Fingered Chords", "c7beca58-d894-452c-8476-872db2173dd9");

        FingeredChordsProcessor(const babelwires::ProjectContext& projectContext);

      protected:
        void processValue(babelwires::UserLogger& userLogger, const babelwires::ValueTreeNode& input,
                          babelwires::ValueTreeNode& output) const override;
    };
} // namespace bw_music
