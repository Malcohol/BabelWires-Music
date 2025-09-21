/**
 * MidiChannel is an int type constrained to the legal range of a MIDI channel (0-15).
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/TypeSystem/registeredType.hpp>
#include <BabelWiresLib/Types/Int/intType.hpp>

namespace smf {
    /// An int type constrained to the legal range of a MIDI channel (0-15).
    class MidiChannel : public babelwires::IntType {
      public:
        REGISTERED_TYPE("MidiChannel", "MIDI Channel", "85f19ba3-f5be-4c81-9632-9227f85baa4e", 1);
        MidiChannel();
    };
} // namespace smf
