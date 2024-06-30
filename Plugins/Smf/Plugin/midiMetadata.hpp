/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <SeqWiresLib/Features/tempoFeature.hpp>

#include <BabelWiresLib/Types/Record/recordType.hpp>
#include <BabelWiresLib/Instance/instanceOfBuiltInTypes.hpp>

namespace smf {
    class MidiMetadata : public babelwires::RecordType {
      public:
        PRIMITIVE_TYPE("midiMetadata", "MIDI Metadata", "5032054f-d1b7-4cb4-8bac-2de60a1bc078", 1);

        MidiMetadata();

        DECLARE_INSTANCE_BEGIN(MidiMetadata)
        DECLARE_INSTANCE_FIELD(Spec, GMSpecType)
        DECLARE_INSTANCE_FIELD_OPTIONAL(Tempo, seqwires::Tempo)
        DECLARE_INSTANCE_FIELD_OPTIONAL(Name, babelwires::StringType)
        DECLARE_INSTANCE_FIELD_OPTIONAL(CopyR, babelwires::StringType)
        DECLARE_INSTANCE_END()
    };

    class MidiMetadataFeature : public babelwires::SimpleValueFeature {
      public:
        MidiMetadataFeature();
    };
} // namespace smf
