/**
 * A PercussionSet corresponding to the instruments of XG Room Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Room percussion set.
    class XgRoomPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGRoomPerc", "Yamaha XG Room Percussion", "4ad1aa44-6e23-4f3e-ae83-75b0b1edad4a", 1);

        XgRoomPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf