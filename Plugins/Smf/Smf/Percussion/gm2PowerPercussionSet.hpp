/**
 * A PercussionSet corresponding to the instruments of GM2 Power Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Power Percussion set.
    class GM2PowerPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2PowerPerc", "General MIDI 2 Power Percussion", "5405b858-e0e7-4aae-877a-557c1cb00826", 1);

        GM2PowerPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf