/**
 * A type corresponding to the contents of a Standard MIDI file.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/midiMetadata.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>
#include <Plugins/Smf/Plugin/recordOfMidiTracks.hpp>

#include <BabelWiresLib/Types/RecordWithVariants/recordWithVariantsType.hpp>
#include <BabelWiresLib/Instance/instance.hpp>

#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>

namespace bw_music {
    class Track;
} // namespace bw_music

namespace smf {

    /// A type corresponding to the contents of a Standard MIDI file.
    class SmfSequence : public babelwires::RecordWithVariantsType {
      public:
        REGISTERED_TYPE("SmfSeqType", "Standard MIDI File", "d4c70fb2-fb67-4e69-82ca-328ec242b0a8", 1);
        SmfSequence(const babelwires::TypeSystem& typeSystem);

        DECLARE_INSTANCE_BEGIN(SmfSequence)
        DECLARE_INSTANCE_FIELD(Meta, MidiMetadata)
        DECLARE_INSTANCE_FIELD(Trcks0, RecordOfMidiTracks);
        DECLARE_INSTANCE_ARRAY_FIELD(Trcks1, MidiTrackAndChannel);
        DECLARE_INSTANCE_END()
    };
}
