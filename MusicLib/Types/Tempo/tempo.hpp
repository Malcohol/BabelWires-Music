/**
 * Tempo is a FixedType for holding a tempo value in bpm.
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

    /// Tempo is a FixedType for holding a tempo value in bpm.
    /// The default tempo is 120.00.
    class MUSICLIB_API Tempo : public babelwires::FixedType {
      public:
        DOWNCASTABLE(Tempo, babelwires::FixedType);
        REGISTERED_TYPE("tempo", "Tempo", "6ee26c7f-ced6-400d-a927-9464a143b39c", 1);
        Tempo()
            : babelwires::FixedType(getThisIdentifier(), 2, getBpmRangeNumerator(),
                                    babelwires::Fixed::assertFromDouble(120.0, 2).getNumerator()) {}

      private:
        static babelwires::Range<babelwires::Fixed::NativeType> getBpmRangeNumerator() {
            const auto bpmRange = TempoValue::getBpmRangeRounded(2);
            return {babelwires::Fixed::assertFromDouble(bpmRange.m_min, 2).getNumerator(),
                    babelwires::Fixed::assertFromDouble(bpmRange.m_max, 2).getNumerator()};
        }
    };

} // namespace bw_music
