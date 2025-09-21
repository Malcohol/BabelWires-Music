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
    class XgSFX2PercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGSFX2Perc", "Yamaha XG SFX 2 Percussion", "217785d3-ac7b-4a2f-9e7b-21e601cf0485", 1);

        XgSFX2PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf