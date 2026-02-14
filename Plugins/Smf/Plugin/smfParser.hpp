/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/Percussion/standardPercussionSets.hpp>
#include <Plugins/Smf/Plugin/smfSequence.hpp>

#include <MusicLib/musicTypes.hpp>

#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/Log/userLogger.hpp>
#include <BaseLib/Utilities/result.hpp>

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

namespace bw_music {
    class PercussionSetWithPitchMap;
}

namespace smf {

    class SmfParser {
      public:
        SmfParser(babelwires::DataSource& dataSource, const babelwires::ProjectContext& projectContext,
                  babelwires::UserLogger& log);
        virtual ~SmfParser();

        babelwires::Result parse();
        std::unique_ptr<babelwires::ValueTreeRoot> getResult() { return std::move(m_result); }

      protected:
        SmfSequence::ConstInstance getSmfSequenceConst() const;
        SmfSequence::Instance getSmfSequence();

        babelwires::ResultT<babelwires::Byte> getNext();
        babelwires::ResultT<babelwires::Byte> peekNext();

        void setGMSpec(GMSpecType::Value spec);

        /// Read the expected byte sequence.
        babelwires::Result readByteSequence(const char* seq);

        /// Reads the header chunk and set some metadata.
        babelwires::Result readHeaderChunk();

        babelwires::Result readFormat0Sequence();
        babelwires::Result readFormat1Sequence();
        babelwires::Result readFormat1SequenceTrack(MidiTrackAndChannel::Instance& track, bool hasMainMetadata = false);

        MidiMetadata::Instance getMidiMetadata();

        class TrackSplitter;

        babelwires::Result readTrack(int trackIndex, TrackSplitter& tracks, bool hasMainMetadata = false);

        babelwires::ResultT<bw_music::ModelDuration> readModelDuration();

        void readTempoEvent(std::uint32_t tempoValue);

        babelwires::ResultT<std::string> readTextMetaEvent(int length);

        babelwires::Result skipBytes(int numBytes);

        template <typename STREAMLIKE> babelwires::Result logByteSequence(STREAMLIKE log, int length);

        babelwires::Result readSysExEvent();
        babelwires::Result readSysExEventContinuation();
        babelwires::Result readSequencerSpecificEvent(int length);

        ///
        babelwires::ResultT<std::uint16_t> readU16();
        babelwires::ResultT<std::uint32_t> readU24();
        babelwires::ResultT<std::uint32_t> readU32();
        babelwires::ResultT<std::uint32_t> readVariableLengthQuantity();

        /// Try to read length bytes into the buffer.
        babelwires::Result readFullMessageIntoBuffer(std::uint32_t length);

        /// A -1 in the message is allowed to be anything.
        template <std::size_t N> bool isMessageBufferMessage(const std::array<std::int16_t, N>& message) const;

        template <typename STREAMLIKE> void logMessageBuffer(STREAMLIKE log) const;

        babelwires::Result readControlChange(unsigned int channelNumber);
        babelwires::Result readProgramChange(unsigned int channelNumber);
        void setBankMSB(unsigned int channelNumber, const babelwires::Byte msbValue);
        void setBankLSB(unsigned int channelNumber, const babelwires::Byte lsbValue);
        void setProgram(unsigned int channelNumber, const babelwires::Byte value);

        // Note: blockNumber is not the same as channel number.
        void setGsPartMode(unsigned int blockNumber, babelwires::Byte value);
        
        void onChangeProgram(unsigned int channelNumber);

        enum KnownPercussionSets { GM_PERCUSSION_KIT, GM2_STANDARD_PERCUSSION_KIT, NUM_KNOWN_PERCUSSION_KITS };

      private:
        const babelwires::ProjectContext& m_projectContext;
        babelwires::DataSource& m_dataSource;
        babelwires::UserLogger& m_userLogger;
        std::unique_ptr<babelwires::ValueTreeRoot> m_result;
        std::vector<babelwires::Byte> m_messageBuffer;

        enum class Format { SMF_FORMAT_0, SMF_FORMAT_1, SMF_FORMAT_2, SMF_UNKNOWN_FORMAT };
            
        Format m_sequenceType;
        int m_numTracks;
        int m_division;

        /// Knowledge of how pitches map to percussion instruments.
        StandardPercussionSets m_standardPercussionSets;

        /// Currently just used to determine which tracks are percussion tracks.
        struct ChannelSetup {
            StandardPercussionSets::ChannelSetupInfo m_channelSetupInfo;
            // This is non-null when the pitches in the data should be interpreted as percussion events from the given
            // kit.
            const bw_music::PercussionSetWithPitchMap* m_kitIfPercussion = nullptr;
        };

        std::array<ChannelSetup, 16> m_channelSetup;
    };

    std::unique_ptr<babelwires::ValueTreeRoot> parseSmfSequence(babelwires::DataSource& dataSource,
                                                              const babelwires::ProjectContext& projectContext,
                                                              babelwires::UserLogger& userLogger);

} // namespace smf
