/**
 * A PercussionSet corresponding to the instruments of GS Jazz Percussion Set.
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
    /// A PercussionSet corresponding to the instruments of GS Jazz Percussion set.
    class SMFLIB_API GsJazzPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("GSJazzPerc", "Roland GS Jazz Percussion", "82bf0d75-260c-46f3-91d0-1d09b5494022", 1);

        GsJazzPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf