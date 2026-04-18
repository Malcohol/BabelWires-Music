/**
 * A PercussionSet corresponding to the instruments of GM2 SFX Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 SFX percussion set.
    class GM2SFXPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2SFXPerc", "General MIDI 2 SFX Percussion", "5d51ad28-2f32-467e-a5be-8caafe3e6b39", 1);

        GM2SFXPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf