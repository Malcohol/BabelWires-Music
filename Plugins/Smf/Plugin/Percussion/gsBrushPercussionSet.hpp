/**
 * A PercussionSet corresponding to the instruments of GS Standard Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/Plugins/Smf/Plugin/smfLibExport.hpp>

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of GS standard percussion set.
    /// There are two such sets, but we do not distinguish them
    class SMFLIB_API GsBrushPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GSBrushPerc", "Roland GS Brush Percussion", "8a2d5bde-833f-43ea-9c6c-86c22174688b", 1);

        GsBrushPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf