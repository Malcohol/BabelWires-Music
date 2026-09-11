/**
 * Represents a value between a minimum and maximum, with a distinguished centred value, using an integer.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/minCentreMaxValue.hpp>

template class bw_music::MinCentreMaxValueT<std::uint8_t>;
template class bw_music::MinCentreMaxValueT<std::uint16_t>;
template class bw_music::MinCentreMaxValueT<std::uint32_t>;
template class bw_music::MinCentreMaxValueT<std::uint64_t>;
