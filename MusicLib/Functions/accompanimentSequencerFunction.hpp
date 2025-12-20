/**
 * Function which join tracks together sequencially.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/

#pragma once

#include <BabelWiresLib/TypeSystem/valueHolder.hpp>
#include <MusicLib/Types/Track/track.hpp>

namespace babelwires {
    class RecordType;
}

namespace bw_music {
    /// The accompanimentTracks are expected to be of a record type, whose fieldIds are all chord types.
    /// The types of the fields are required to be the same.
    /// The result value has the same type as the fields, and consists of the tracks in those type concatenated in a
    /// sequence defined by the chordTrack. The tracks are truncated or repeated as necessary to fit the chord
    /// durations. Any non-track contents in the field values simply keep the value from the first chord.
    babelwires::ValueHolder accompanimentSequencerFunction(const babelwires::TypeSystem& typeSystem,
                                                           const babelwires::RecordType& typeOfAccompanimentTracks,
                                                           const babelwires::ValueHolder& accompanimentTracks,
                                                           const bw_music::Track& chordTrack);
} // namespace bw_music
