/**
 * A function which extracts a list of monophonic tracks from a track.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Types/Track/track.hpp>

#include <BabelWiresLib/Types/Enum/enumWithCppEnum.hpp>
#include <BabelWiresLib/TypeSystem/registeredType.hpp>

namespace bw_music {
#define MONOPHONIC_SUBTRACK_POLICY(X)                                                                                  \
    X(High, "Higher pitches", "00b17c7c-f520-4058-b57d-15f9cc89a890")                                                  \
    X(Low, "Lower pitches", "9a0b8072-11a2-492d-a21a-4e568c3483e0")                                                    \
    X(HighEv, "Higher pitches evict", "37f9408a-2e91-47d7-94ed-9e4e2086a4b6")                                          \
    X(LowEv, "Lower pitches evict", "7068c9fd-d80a-47fa-8f82-6b000cd1e0be")

    /// The enum that determines the algorithm used.
    class MonophonicSubtracksPolicyEnum : public babelwires::EnumType {
      public:
        REGISTERED_TYPE("MonoPolicy", "Monophonic Subtracks Policy", "d9ae8da5-3001-45ff-b2ce-4375f7d18afd", 1);
        MonophonicSubtracksPolicyEnum();

        ENUM_DEFINE_CPP_ENUM(MONOPHONIC_SUBTRACK_POLICY);
    };

    struct MonophonicSubtracksResult {
        std::vector<Track> m_noteTracks;
        Track m_other;
    };

    MonophonicSubtracksResult getMonophonicSubtracks(const Track& trackIn, int numTracks,
                                                     MonophonicSubtracksPolicyEnum::Value policy);
} // namespace bw_music
