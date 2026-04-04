/**
 * A MonophonicNoteIterator iterates over a track, returning note events which never overlap.
 * 
 * (C) 2021 Malcolm Tyrrell
 * 
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Utilities/filteredTrackIterator.hpp>

namespace bw_music {
    /// This returns note events which never overlap.
    /// This just uses a first-come-first-serve policy, so the resulting track will probably not be as useful
    /// as the tracks which come out of the MonophonicSubtrackFunction / MonophonicSubtrackProcessor. 
    /// Any note started when the active note is playing is ignored.
    struct MUSICLIB_API MonophonicNoteIterator : FilteredTrackIterator<NoteEvent> {
        enum InteriorEventFilter { OnOffOnly, AllEvents };

        /// If onOffOnly is true, then only NoteOnEvent and NoteOffEvent events will be returned.
        MonophonicNoteIterator(const Track& track, InteriorEventFilter interiorEventFilter);

        bool isEventOfInterest(const TrackEvent& event) override;

        bool m_noteIsActive = false;
        Pitch m_activePitch = 0;
        InteriorEventFilter m_interiorEventFilter = OnOffOnly;
    };

    /// Return a span of iterators which only return non-overlapping notes.
    MUSICLIB_API babelwires::Span<MonophonicNoteIterator>
    iterateOverMonotonicNotes(const Track& track, MonophonicNoteIterator::InteriorEventFilter interiorEventFilter);
} // namespace bw_music
