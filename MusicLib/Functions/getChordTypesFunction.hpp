/**
 * Function which determines the set of chords used by a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/MusicLib/musicLibExport.hpp>

#include <BabelWiresLib/TypeSystem/registeredType.hpp>

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/chord.hpp>

#include <BaseLib/Result/result.hpp>

#include <set>

namespace bw_music {
    /// Get the set of chord types used in the given chord track.
    MUSICLIB_API babelwires::ResultT<std::set<ChordType::Value>> getChordTypesFunction(const Track& chordTrack);

} // namespace bw_music
