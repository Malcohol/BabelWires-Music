/**
 * Function which determines the set of chords used by a chord track.
 *
 * (C) 2025 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 */
#include <MusicLib/Functions/getChordTypesFunction.hpp>

#include <MusicLib/Types/Track/track.hpp>
#include <MusicLib/Types/Track/TrackEvents/chordEvents.hpp>

std::set<bw_music::ChordType::Value> bw_music::getChordTypesFunction(const Track& chordTrack) {
    std::set<ChordType::Value> chordTypes;

    // Iterate through the chord track and collect unique chord types.
    for (const auto& event : chordTrack) {
        if (const auto chordEvent = event.tryAs<ChordOnEvent>()) {
            chordTypes.insert(chordEvent->m_chord.m_chordType);
        }
    }

    return chordTypes;
}
