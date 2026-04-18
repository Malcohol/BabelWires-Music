/**
 * A PercussionSet corresponding to the instruments of GM2 Brush Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GM2 Brush Percussion Set.
    class GM2BrushPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GM2BrushPerc", "General MIDI 2 Brush Percussion", "9686b521-ffb2-4275-b98a-e7e6cdc1f91d", 1);
        GM2BrushPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf
