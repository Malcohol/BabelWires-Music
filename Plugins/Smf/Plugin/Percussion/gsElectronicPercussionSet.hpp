/**
 * A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS Electronic Percussion Set.
    /// This can also serve for the GS Dance set.
    class GsElectronicPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GSElecPerc", "Roland GS Electronic Percussion", "9ed6fdd8-f4d3-4944-b24f-a34d85d67fdb",
                       1);

        GsElectronicPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf