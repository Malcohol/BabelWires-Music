/**
 * A PercussionSet corresponding to the instruments of XG Standard 1 Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Standard 1 percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgStandard1PercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGStand1Perc", "Yamaha XG Standard 1 Percussion", "4c094420-0a8e-4dfa-b315-2bdb516d489a", 1);

        XgStandard1PercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf