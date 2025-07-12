/**
 * A PercussionSet corresponding to the instruments of GM2 Analog Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Analog Percussion Set.
    class GM2AnalogPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2AnalogPerc", "General MIDI 2 Analog Percussion", "a077c9c6-e66d-46d2-b5e2-0e839b99ae70", 1);
        GM2AnalogPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf