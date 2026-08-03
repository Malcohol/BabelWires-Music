/**
 * Represents a value in the asymmetric range [0,... 0x80000000,.. 0xFFFFFFFF].
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
