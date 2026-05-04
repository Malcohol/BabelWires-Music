/**
 * A PercussionSet corresponding to the instruments of GM2 Orchestra Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Orchestra percussion set.
    class GM2OrchestraPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2OrchPerc", "General MIDI 2 Orchestra Percussion", "6a77f085-35c7-409c-92e1-5f4a20f3c750", 1);

        GM2OrchestraPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf