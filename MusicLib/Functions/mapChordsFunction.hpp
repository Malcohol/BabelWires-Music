/**
 * Function which applies maps to chord events.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <MusicLib/Types/Track/track.hpp>

namespace babelwires {
    class MapValue;
    class TypeSystem;
}

namespace bw_music {
    babelwires::TypeExp getMapChordFunctionSourceTypeExp();
    babelwires::TypeExp getMapChordFunctionTargetTypeExp();

    /// Apply maps to chord events in the track.
    /// You can specify a chord that should be active when no chord in the sourceTrack is active
    /// by having blanks in the source map.
    Track mapChordsFunction(const babelwires::TypeSystem& typeSystem, const Track& sourceTrack, const babelwires::MapValue& chordMapValue);
}
