/**
 * Representation of a Standard MIDI File as a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiMetadata.hpp>

#include <Plugins/Smf/Plugin/smfFormat.hpp>

#include <MusicLib/Types/tempo.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/String/stringType.hpp>

#include <Common/Identifiers/registeredIdentifier.hpp>

smf::MidiMetadata::MidiMetadata(const babelwires::TypeSystem& typeSystem)
    : babelwires::RecordType(getThisType(), typeSystem,
          {{BW_SHORT_ID("Spec", "MIDI Spec", "15a9fa85-f2c6-4e68-8691-fefd64ca1233"),
            GMSpecType::getThisType()},
           {BW_SHORT_ID("Tempo", "Tempo", "3ef804e9-e34a-4a25-b6bf-ce7597d9d90b"),
            bw_music::Tempo::getThisType(), babelwires::RecordType::Optionality::optionalDefaultInactive},
           {BW_SHORT_ID("Name", "Name", "c2e4910f-d006-4a93-97a7-ae5973157ec8"),
            babelwires::StringType::getThisType(), babelwires::RecordType::Optionality::optionalDefaultInactive},
           {BW_SHORT_ID("CopyR", "Copyright", "a59dc914-d060-4f03-be83-5804fc4d6b6a"),
            babelwires::StringType::getThisType(),
            babelwires::RecordType::Optionality::optionalDefaultInactive}}) {}
