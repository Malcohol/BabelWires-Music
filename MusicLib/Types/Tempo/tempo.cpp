/**
 * Tempo is a FixedType for holding a tempo value in bpm to 2 decimal places.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Tempo/tempo.hpp>

namespace {
    babelwires::Range<babelwires::Fixed::NativeType> getBpmRangeNumerator() {
        const auto bpmRange = bw_music::TempoValue::getBpmRangeRounded(2);
        return {babelwires::Fixed::assertFromDouble(bpmRange.m_min, 2).getNumerator(),
                babelwires::Fixed::assertFromDouble(bpmRange.m_max, 2).getNumerator()};
    }
} // namespace

bw_music::Tempo::Tempo()
    : babelwires::FixedType(getThisIdentifier(), 2, getBpmRangeNumerator(),
                            babelwires::Fixed::assertFromDouble(120.0, 2).getNumerator()) {}
