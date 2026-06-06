/**
 * A queryable interface for transposable track events.
 *
 * (C) 2026 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/musicLibExport.hpp>

#include <MusicLib/Utilities/musicUtilities.hpp>

#include <BaseLib/Utilities/interfaceQueryable.hpp>

namespace bw_music {

    /// Capability interface for track events whose pitches can be transposed.
    struct MUSICLIB_API Transposable {
        QUERYABLE_INTERFACE(Transposable);
        virtual ~Transposable() = default;

        /// If it makes sense, transpose the pitch or pitches described by this event by the given number of semitones.
        /// Return false if the event has become invalidated.
        virtual bool transpose(int pitchOffset,
                               TransposeOutOfRangePolicy outOfRangePolicy = TransposeOutOfRangePolicy::Discard) = 0;
    };

} // namespace bw_music