/**
 * MidiTrackAndChannelArray is an array of MidiTrackAndChannel records.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

smf::MidiTrackAndChannelArray::MidiTrackAndChannelArray(const babelwires::TypeSystem& typeSystem)
    : ArrayType(getThisIdentifier(), typeSystem.getEntryByType<smf::MidiTrackAndChannel>(), 1, 16, 1) {}
