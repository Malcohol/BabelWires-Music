/**
 *
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <MusicLib/Percussion/percussionSetWithPitchMap.hpp>
#include <Plugins/Smf/Plugin/gmSpec.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/TypeSystem/typePtr.hpp>

#include <unordered_set>

namespace babelwires {
    struct ProjectContext;
}

namespace smf {
    class StandardPercussionSets {
      public:
        StandardPercussionSets(const babelwires::ProjectContext& projectContext);

        /// Get the default set for each channel in the given spec.
        const bw_music::PercussionSetWithPitchMap* getDefaultPercussionSet(GMSpecType::Value gmSpec, int channelNumber);

        // TODO This isn't percussion specific.
        struct ChannelSetupInfo {
            babelwires::Byte m_bankMSB = 0;
            babelwires::Byte m_bankLSB = 0;
            babelwires::Byte m_program = 0;
            /// This is the part corresponding to this channel, irrespective of the part mapping.
            babelwires::Byte m_gsPartMode = 0;
        };

        /// Get the percussion set specified by the given parameters, or nullptr if a percussion set is not specified.
        const bw_music::PercussionSetWithPitchMap* getPercussionSetFromChannelSetupInfo(GMSpecType::Value gmSpec, ChannelSetupInfo channelSetupInfo);

        /// Find the percussion set which contains the most instruments from the given set, and is suitable for use in
        /// the given channel.
        const bw_music::PercussionSetWithPitchMap* getBestPercussionSet(GMSpecType::Value gmSpec, int channelNumber,
                                                  const std::unordered_set<babelwires::ShortId>& instrumentsInUse,
                                                  std::unordered_set<babelwires::ShortId>& excludedInstrumentsOut);

        /// Get setup information for the given percussionSet if any is required. 
        /// A null argument is allowed, in which case an empty optional is returned.
        std::optional<ChannelSetupInfo> getChannelSetupInfoFromPercussionSet(const bw_music::PercussionSetWithPitchMap* percussionSet, int channelNumber);

      private:
        enum KnownPercussionSets {
            GM_PERCUSSION_SET,

            GM2_STANDARD_PERCUSSION_SET,
            GM2_ANALOG_PERCUSSION_SET,
            GM2_BRUSH_PERCUSSION_SET,
            GM2_ELECTRONIC_PERCUSSION_SET,
            GM2_JAZZ_PERCUSSION_SET,
            GM2_ORCHESTRA_PERCUSSION_SET,
            GM2_POWER_PERCUSSION_SET,
            GM2_ROOM_PERCUSSION_SET,
            GM2_SFX_PERCUSSION_SET,

            GS_STANDARD_1_PERCUSSION_SET,
            GS_ROOM_PERCUSSION_SET,
            GS_POWER_PERCUSSION_SET,
            GS_ELECTRONIC_PERCUSSION_SET,
            GS_808_909_PERCUSSION_SET,
            GS_JAZZ_PERCUSSION_SET,
            GS_BRUSH_PERCUSSION_SET,
            GS_ORCHESTRA_PERCUSSION_SET,
            GS_SFX_PERCUSSION_SET,

            XG_STANDARD_1_PERCUSSION_SET,
            XG_ROOM_PERCUSSION_SET,
            XG_ROCK_PERCUSSION_SET,
            XG_ELECTRO_PERCUSSION_SET,
            XG_ANALOG_PERCUSSION_SET,
            XG_JAZZ_PERCUSSION_SET,
            XG_BRUSH_PERCUSSION_SET,
            XG_CLASSIC_PERCUSSION_SET,
            XG_SFX_1_PERCUSSION_SET,
            XG_SFX_2_PERCUSSION_SET,

            NUM_KNOWN_PERCUSSION_SETS,
            NOT_PERCUSSION = NUM_KNOWN_PERCUSSION_SETS,

            // Ranges

            GM2_SETS_START = GM2_STANDARD_PERCUSSION_SET,
            GM2_SETS_END = GM2_SFX_PERCUSSION_SET,

            GS_SETS_START = GS_STANDARD_1_PERCUSSION_SET,
            GS_SETS_END = GS_SFX_PERCUSSION_SET,

            XG_SETS_START = XG_STANDARD_1_PERCUSSION_SET,
            XG_SETS_END = XG_SFX_2_PERCUSSION_SET,
        };

        /// Ensure the m_instrumentSets array is populated.
        void ensureInstrumentSets();

        /// Get the set of instruments in instrumentsInUse missing from the percussion set at the given index.
        void getExcludedInstruments(int percussionSetIndex, const std::unordered_set<babelwires::ShortId>& instrumentsInUse,
                                                  std::unordered_set<babelwires::ShortId>& excludedInstrumentsOut);

        /// From the range of known percussion, select the best percussion set.
        const bw_music::PercussionSetWithPitchMap* getBestPercussionSetInRange(int startIndex, int endIndex,
                                                  const std::unordered_set<babelwires::ShortId>& instrumentsInUse,
                                                  std::unordered_set<babelwires::ShortId>& excludedInstrumentsOut);

        /// Find the percussion set in the array of known percussion sets. Asserts if not found.
        KnownPercussionSets getKnownPercussionSetFromPercussionSet(const bw_music::PercussionSetWithPitchMap* percussionSet);

        std::optional<ChannelSetupInfo> getChannelSetupInfoFromKnownPercussionSet(KnownPercussionSets percussionSet, int channelNumber);

      private:
        std::array<babelwires::TypePtrT<const bw_music::PercussionSetWithPitchMap>, NUM_KNOWN_PERCUSSION_SETS> m_knownSets;

        /// This is populated on demand.
        std::array<std::unordered_set<babelwires::ShortId>, NUM_KNOWN_PERCUSSION_SETS> m_instrumentSets;
    };

} // namespace smf
