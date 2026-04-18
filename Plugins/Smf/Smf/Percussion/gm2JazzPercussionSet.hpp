/**
 * A PercussionSet corresponding to the instruments of GM2 Jazz Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Jazz percussion set.
    class GM2JazzPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2JazzPerc", "General MIDI 2 Jazz Percussion", "19ff2ea7-10e7-47d1-866d-ae241be9f3cc", 1);
        GM2JazzPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf