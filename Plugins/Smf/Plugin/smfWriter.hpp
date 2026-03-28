/**
 * Write a Standard MIDI File based on the contents of a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Domains/Music/Plugins/Smf/Plugin/smfLibExport.hpp>

#include <Plugins/Smf/Plugin/Percussion/standardPercussionSets.hpp>
#include <Plugins/Smf/Plugin/smfSequence.hpp>

#include <MusicLib/musicTypes.hpp>

#include <cstdint>
#include <ostream>

namespace babelwires {
    struct UserLogger;
}

namespace bw_music {
    class Track;
    class TrackEvent;
} // namespace bw_music

namespace smf {

    class SMFLIB_API SmfWriter {
      public:
        SmfWriter(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                  const babelwires::ValueTreeRoot& sequence, std::ostream& output);

        void write();

      protected:
        SmfSequence::ConstInstance getSmfSequenceConst() const;

        void writeUint16(std::uint16_t i);
        void writeUint24(std::uint32_t i);
        void writeUint32(std::uint32_t i);
        void writeVariableLengthQuantity(std::uint32_t i);
        void writeModelDuration(const bw_music::ModelDuration& d);

        /// Returns true if the event was written.
        enum class WriteTrackEventResult { Written, WrongCategory, NotInPercussionSet };
        WriteTrackEventResult writeTrackEvent(int channelNumber, bw_music::ModelDuration timeSinceLastEvent,
                                              const bw_music::TrackEvent& e);

        void writeTempoEvent(int bpm);

        /// type is the integer 0..15 which defines which type of text meta-event should be issued.
        void writeTextMetaEvent(int type, std::string text);

        using ChannelAndTrack = std::tuple<unsigned int, const bw_music::Track*>;

        void applyToAllTracks(std::function<void(unsigned int, const bw_music::Track&)> function);

        void writeNotes(const std::vector<ChannelAndTrack>& tracks);

        void writeHeaderChunk(unsigned int numTracks);

        /// Write the events for the given track.
        void writeTrack(const std::vector<ChannelAndTrack>& tracks, bool includeGlobalSetup);

        /// Write non-channel-specific setup information.
        void writeGlobalSetup();

        /// Determine from the events in the tracks what percussion kit (allowed for the channelNumber) includes the
        /// largest number of the events.
        void setUpPercussionKit(const std::unordered_set<babelwires::ShortId>& instrumentsInUse, int channelNumber);

        void setUpPercussionSets();

        template <std::size_t N> void writeMessage(const std::array<std::uint8_t, N>& message);

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::UserLogger& m_userLogger;
        const babelwires::ValueTreeRoot& m_smfFeature;
        std::ostream& m_ostream;
        std::ostream* m_os;
        /// Always use metrical time. Quater-note division.
        int m_division;

        StandardPercussionSets m_standardPercussionSets;

        /// Currently just used to determine which tracks are percussion tracks.
        struct SMFLIB_API ChannelSetup {
            // This is non-null when the pitches in the data should be interpreted as percussion events from the given
            // kit.
            const bw_music::PercussionSetWithPitchMap* m_kitIfPercussion = nullptr;
            // Has channel set-up information been written for this channel yet?
            // (Since more than one track can correspond to a channel, we only want to write this for the first track
            // for each channel.)
            bool m_setupWritten = false;
        };

        std::array<ChannelSetup, 16> m_channelSetup;
    };

    SMFLIB_API void writeToSmf(const babelwires::ProjectContext& projectContext, babelwires::UserLogger& userLogger,
                    const babelwires::ValueTreeRoot& sequence, std::ostream& output);

} // namespace smf
