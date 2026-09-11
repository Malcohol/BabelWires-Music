/**
 * Represents a value between a minimum and maximum, with a distinguished centred value, using an integer.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/minCentredMaxValue.hpp>

template class bw_music::MinCentredMaxValueT<std::uint8_t>;
template class bw_music::MinCentredMaxValueT<std::uint16_t>;
template class bw_music::MinCentredMaxValueT<std::uint32_t>;
template class bw_music::MinCentredMaxValueT<std::uint64_t>;
