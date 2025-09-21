/**
 * A PercussionSet corresponding to the instruments of XG SFX 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG SFX 1 percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgSFX1PercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGSFX1Perc", "Yamaha XG SFX 1 Percussion", "fb2f88a2-de91-418d-8b8c-26aaac97c1ba", 1);

        XgSFX1PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf