/**
 * A PercussionSet corresponding to the instruments of GS Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS standard percussion set.
    /// There are two such sets, but we do not distinguish them
    class GsSFXPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GSSFXPerc", "Roland GS SFX Percussion", "135823fa-ec1b-427e-9e03-f888be7383c8", 1);

        GsSFXPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf