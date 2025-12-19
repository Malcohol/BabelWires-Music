/**
 * Some utility functions useful for manipulating music sequence data.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Utilities/musicUtilities.hpp>

#include <MusicLib/Types/Track/track.hpp>

int bw_music::getMinimumDenominator(const Track& track) {
    int denominator = 1;

    for (const auto& e : track) {
        denominator = babelwires::lcm(denominator, e.getTimeSinceLastEvent().getDenominator());
    }

    return denominator;
}

std::optional<int> bw_music::transposePitch(int pitch, int offset, TransposeOutOfRangePolicy outOfRangePolicy, int lowerLimit, int upperLimit) {
    int newPitch = pitch + offset;

    if (newPitch < lowerLimit) {
        switch (outOfRangePolicy) {
            case TransposeOutOfRangePolicy::Discard:
                return std::nullopt;
            case TransposeOutOfRangePolicy::MapToNearestOctave:
                newPitch = lowerLimit + 12 + ((newPitch - lowerLimit) % 12);
                break;
        }
    } else if (newPitch > upperLimit) {
        switch (outOfRangePolicy) {
            case TransposeOutOfRangePolicy::Discard:
                return std::nullopt;
            case TransposeOutOfRangePolicy::MapToNearestOctave:
                newPitch = upperLimit - 12 + ((newPitch - upperLimit) % 12);
                break;
        }
    }
    return newPitch;
}
