/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/ValueTree/valueTreeNode.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Array/arrayType.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>

namespace smf {
    // An array of MidiTrackAndChannel records.
    // TODO: Just use a type constructor for this rather than a C++ class?
    class MidiTrackAndChannelArray : public babelwires::ArrayType {
      public:
        REGISTERED_TYPE("MidiTrackArray", "MIDI Track and Channels", "b5462658-be68-4570-b0a5-d2dcb03f4d8e", 1);
        MidiTrackAndChannelArray();
    };
} // namespace smf
