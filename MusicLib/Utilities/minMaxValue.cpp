/**
 * Represents a value between a minimum and a maximum, using an integer evenly distributed across the range.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/minMaxValue.hpp>

template class bw_music::MinMaxValueT<std::uint8_t>;
template class bw_music::MinMaxValueT<std::uint16_t>;
template class bw_music::MinMaxValueT<std::uint32_t>;
template class bw_music::MinMaxValueT<std::uint64_t>;