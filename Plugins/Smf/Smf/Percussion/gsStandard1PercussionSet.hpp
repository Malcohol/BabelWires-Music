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
    class GsStandard1PercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GSStand1Perc", "Roland GS Standard 1 Percussion", "a1fb9fe9-98d0-4f56-ab75-49b9c936d246", 1);

        GsStandard1PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf