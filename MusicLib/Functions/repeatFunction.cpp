/**
 * A function which repeats sequence data a number of times.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Functions/repeatFunction.hpp>

#include <MusicLib/Functions/appendTrackFunction.hpp>

#include <BabelWiresLib/ValueTree/modelExceptions.hpp>

bw_music::Track bw_music::repeatTrack(const Track& trackIn, int count) {
    if (count < 0) {
        throw babelwires::ModelException() << "You cannot have repeat a negative number of times";
    }

    Track trackOut;

    for (int i = 0; i < count; ++i) {
        appendTrack(trackOut, trackIn);
    }

    return trackOut;
}
