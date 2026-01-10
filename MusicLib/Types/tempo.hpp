/**
 * Tempo is an IntType for holding a tempo value in bpm.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <BabelWiresLib/Types/Int/intType.hpp>

namespace bw_music {

    /// Tempo is an IntType for holding a tempo value in bpm.
    /// Tempos are limited to integral bpm.
    /// The default tempo is 120.
    class Tempo : public babelwires::IntType {
      public:
        REGISTERED_TYPE("tempo", "Tempo", "6ee26c7f-ced6-400d-a927-9464a143b39c", 1);
        Tempo() : babelwires::IntType(getThisIdentifier(), {0, 255}, 120) {}
    };

} // namespace bw_music
