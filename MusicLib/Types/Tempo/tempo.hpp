/**
 * Tempo is a FixedType for holding a tempo value in bpm to 2 decimal places.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Utilities/tempoValue.hpp>

#include <BabelWiresLib/Types/Fixed/fixedType.hpp>

namespace bw_music {

    /// Tempo is a FixedType for holding a tempo value in bpm to 2 decimal places.
    /// The range is defined by TempoValue::getBpmRangeRounded(2) and the default is 120.00.
    class MUSICLIB_API Tempo : public babelwires::FixedType {
      public:
        DOWNCASTABLE(Tempo, babelwires::FixedType);
        REGISTERED_TYPE("tempo", "Tempo", "6ee26c7f-ced6-400d-a927-9464a143b39c", 1);
        Tempo();
    };

} // namespace bw_music
