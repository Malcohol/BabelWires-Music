/**
 * A PercussionSet corresponding to the instruments of XG Electro Percussion Set.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace smf {
    /// A PercussionSet corresponding to the instruments of XG Electro percussion set.
    class XgElectroPercussionSet : public bw_music::PercussionSetWithPitchMap {
      public:
        REGISTERED_TYPE("XGElectroPerc", "Yamaha XG Electro Percussion", "eb1c03a3-292c-4bc9-b932-ce1c989e02ca", 1);

        XgElectroPercussionSet(const bw_music::BuiltInPercussionInstruments& builtInInstruments);
    };
} // namespace smf