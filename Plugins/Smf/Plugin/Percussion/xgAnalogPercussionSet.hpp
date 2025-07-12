/**
 * A PercussionSet corresponding to the instruments of XG Analog Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Analog percussion set.
    /// This also works as the XG Standard 2 percussion set.
    class XgAnalogPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGAnalogPerc", "Yamaha XG Analog Percussion", "f172fe57-7ec0-43de-9031-3a4ebe3da84d", 1);

        XgAnalogPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf