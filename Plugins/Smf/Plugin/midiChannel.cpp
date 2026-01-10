/**
 * MidiChannel is an int type constrained to the legal range of a MIDI channel (0-15).
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiChannel.hpp>

smf::MidiChannel::MidiChannel()
    : IntType(getThisType(), {0, 15}, 0) {}
