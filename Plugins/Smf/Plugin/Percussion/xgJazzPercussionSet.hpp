/**
 * A PercussionSet corresponding to the instruments of XG Jazz Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Jazz percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgJazzPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGJazzPerc", "Yamaha XG Jazz Percussion", "101d8344-28c4-4045-83aa-52dc87fe5235", 1);

        XgJazzPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf