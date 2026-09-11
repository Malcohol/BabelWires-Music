/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#pragma once

#include <Smf/Parsing/smfEventConsumer.hpp>
#include <Smf/Parsing/smfTrackEventConsumer.hpp>
#include <Smf/Percussion/standardPercussionSets.hpp>
#include <Smf/gmSpec.hpp>
#include <Smf/smfSequence.hpp>

#include <MusicLib/Types/Track/TrackEvents/trackEventCommon.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/tempoValue.hpp>
#include <MusicLib/musicTypes.hpp>

#include <BaseLib/IO/dataSource.hpp>
#include <BaseLib/Log/userLogger.hpp>
#include <BaseLib/Result/result.hpp>
#include <BaseLib/Text/text.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <vector>

namespace bw_music {
    class PercussionSetWithPitchMap;
}

namespace smf {

    /// The Music-domain consumer which builds an SmfSequence feature tree from the events
    /// fired by an SmfByteParser. The SmfByteParser decomposes the bytes into MIDI messages;
    /// this class interprets them (percussion kits, 14-bit controllers, global tempo,
    /// GM/GS/XG detection, metadata) and assembles the result.
    ///
    /// Shared mutable state (the global tempo map, per-channel state, the percussion sets,
    /// and the result tree) lives here. Each track is handled by a TrackConsumer (see
    /// onTrack) which holds a reference back to this object.
    class SmfConsumer : public SmfEventConsumer {
      public:
        SmfConsumer(const babelwires::Context& context, babelwires::UserLogger& log);

        babelwires::Result onSequenceStart(std::uint16_t numTracks, std::uint16_t format,
                                           std::uint16_t division) override;

        std::unique_ptr<SmfTrackEventConsumer> onTrack(std::uint16_t trackIndex) override;

        /// Called after all tracks have been parsed to assemble the output feature tree
        /// (including the global tempo track).
        babelwires::Result finalize();

        std::unique_ptr<babelwires::ValueTreeRoot> getResult() { return std::move(m_result); }

      private:
        SmfSequence::ConstInstance getSmfSequenceConst() const;
        SmfSequence::Instance getSmfSequence();
        MidiMetadata::Instance getMidiMetadata();

        void setGMSpec(GMSpecType::Value spec);
        void setBankMSB(unsigned int channelNumber, babelwires::Byte msbValue);
        void setBankLSB(unsigned int channelNumber, babelwires::Byte lsbValue);
        void setProgram(unsigned int channelNumber, babelwires::Byte value);
        // Note: blockNumber is not the same as channel number.
        void setGsPartMode(unsigned int blockNumber, babelwires::Byte value);
        void onChangeProgram(unsigned int channelNumber);

        /// This can fail if the tempo is 0.
        babelwires::Result readTempoEvent(int trackIndex, bw_music::ModelDuration absoluteTime,
                                          std::uint32_t tempoValue);

        /// Interpret a SysEx message for GM/GS/XG detection.
        void interpretSysExForGMSpec(std::span<const std::uint8_t> data);

        /// Convert raw MIDI ticks to a ModelDuration using this file's division.
        bw_music::ModelDuration ticksToDuration(std::uint64_t ticks) const;

        /// Assemble the output tracks from the per-track normalized data, according to format.
        babelwires::Result buildOutputTracks();

      private:
        /// Per-track event consumer. Splits a MIDI track's channel events into per-channel
        /// tracks and reports interpretation-relevant events back to the owning SmfConsumer.
        class TrackConsumer : public SmfTrackEventConsumer {
          public:
            TrackConsumer(SmfConsumer& owner, std::uint16_t trackIndex, bool hasMainMetadata);

            /// The byte parser destroys this once the track is fully parsed (or the consumer
            /// signalled Done). The destructor finalizes the track if that has not already
            /// happened (see finalizeTrack).
            ~TrackConsumer();

            babelwires::ResultT<EventHandlingResult> onNoteOn(TimeInfo timeInfo, std::uint8_t channel4,
                                                              std::uint8_t pitch7, std::uint8_t velocity7) override;
            babelwires::ResultT<EventHandlingResult> onNoteOff(TimeInfo timeInfo, std::uint8_t channel4,
                                                               std::uint8_t pitch7, std::uint8_t velocity7) override;
            babelwires::ResultT<EventHandlingResult> onPolyphonicAftertouch(TimeInfo timeInfo, std::uint8_t channel4,
                                                                            std::uint8_t pitch7,
                                                                            std::uint8_t pressure7) override;
            babelwires::ResultT<EventHandlingResult> onChannelPressure(TimeInfo timeInfo, std::uint8_t channel4,
                                                                       std::uint8_t pressure7) override;
            babelwires::ResultT<EventHandlingResult> onPitchBend(TimeInfo timeInfo, std::uint8_t channel4,
                                                                 std::uint16_t value14) override;
            babelwires::ResultT<EventHandlingResult> onControlChange(TimeInfo timeInfo, std::uint8_t channel4,
                                                                     std::uint8_t controller7,
                                                                     std::uint8_t value7) override;
            babelwires::ResultT<EventHandlingResult> onProgramChange(TimeInfo timeInfo, std::uint8_t channel4,
                                                                     std::uint8_t program7) override;
            babelwires::ResultT<EventHandlingResult> onTempoEvent(TimeInfo timeInfo,
                                                                  std::uint32_t tempoValue24) override;
            babelwires::ResultT<EventHandlingResult> onCopyright(TimeInfo timeInfo,
                                                                 std::span<const std::uint8_t> copyright) override;
            babelwires::ResultT<EventHandlingResult> onSequenceOrTrackName(TimeInfo timeInfo,
                                                                           std::span<const std::uint8_t> name) override;
            babelwires::ResultT<EventHandlingResult> onSysExEvent(TimeInfo timeInfo,
                                                                  std::span<const std::uint8_t> data) override;
            babelwires::ResultT<EventHandlingResult> onEndOfTrack(TimeInfo timeInfo) override;

          private:
            struct PerChannelInfo {
                bw_music::TrackBuilder m_track;
                bw_music::ModelDuration m_timeOfLastEvent;
                bw_music::ModelDuration m_trackDuration = 0;
            };

            PerChannelInfo* getChannel(unsigned int channelNumber);

            bool addNoteOn(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent,
                           bw_music::Pitch pitch, bw_music::VelocityStorage velocity);
            bool addNoteOff(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent,
                            bw_music::Pitch pitch, bw_music::VelocityStorage velocity);

            template <typename EVENT_TYPE, typename... ARGS>
            bool addEvent(unsigned int channelNumber, bw_music::ModelDuration timeSinceLastTrackEvent, ARGS&&... args) {
                return addEvent(channelNumber, EVENT_TYPE{timeSinceLastTrackEvent, std::forward<ARGS>(args)...});
            }
            bool addEvent(unsigned int channelNumber, bw_music::TrackEvent&& event);

            /// All channels share the duration of the MIDI track.
            void setDurationsForAllChannels(bw_music::ModelDuration timeToEndOfTrackEvent);

            /// Finish the per-channel tracks and report them to the owner. Called from
            /// onEndOfTrack, and from the destructor as clean-up when the track ended without
            /// an end-of-track event. Does nothing if the track has already been finalized.
            void finalizeTrack();

            /// Convert the interface's tick-based TimeInfo into the ModelDuration since the
            /// last handled event.
            bw_music::ModelDuration timeSinceLastHandledEvent(TimeInfo timeInfo) const;

          private:
            SmfConsumer& m_owner;
            std::uint16_t m_trackIndex;
            bool m_hasMainMetadata;

            bool m_finalized = false;
            bw_music::ModelDuration m_timeSinceStart = 0;
            std::array<std::unique_ptr<PerChannelInfo>, 16> m_channels;
        };

      private:
        const babelwires::Context& m_projectContext;
        babelwires::UserLogger& m_userLogger;
        std::unique_ptr<babelwires::ValueTreeRoot> m_result;

        int m_numTracks = -1;
        int m_division = -1;
        std::uint16_t m_format = 0;

        struct NormalizedTempoEvent {
            int m_trackIndex;
            bw_music::TempoValue m_tempo;
        };
        std::map<bw_music::ModelDuration, NormalizedTempoEvent> m_globalTempoEvents;

        /// Knowledge of how pitches map to percussion instruments.
        StandardPercussionSets m_standardPercussionSets;

        /// Information about the state of each MIDI channel.
        struct ChannelState {
            StandardPercussionSets::ChannelSetupInfo m_channelSetupInfo;
            // This is non-null when the pitches in the data should be interpreted as percussion events
            // from the given kit.
            const bw_music::PercussionSetWithPitchMap* m_kitIfPercussion = nullptr;

            // Cached MSB/LSB values for the 14-bit controllers.
            struct ControllerState {
                std::optional<babelwires::Byte> m_msb;
                std::optional<babelwires::Byte> m_lsb;

                template <typename STORAGE>
                babelwires::ResultT<std::optional<STORAGE>> updateWithNewValue(babelwires::Byte value, bool isLsb);
            };

            ControllerState m_volume;
            ControllerState m_pan;
            ControllerState m_expression;

        };
        std::array<ChannelState, 16> m_channelState;

        /// The normalized per-track output accumulated as each track finishes, keyed by track
        /// index. Since the tracks are parsed in global time order, their end-of-track events can
        /// be reached in any order: keying by index ensures the output preserves the file's
        /// track order.
        struct Format1TrackData {
            unsigned int m_channelNumber;
            bw_music::Track m_track;
            std::vector<std::pair<unsigned int, bw_music::Track>> m_extraTracks;
        };
        std::map<std::uint16_t, Format1TrackData> m_normalizedTracks;
    };

    babelwires::ResultT<std::unique_ptr<babelwires::ValueTreeRoot>>
    parseSmfSequence(babelwires::DataSource& dataSource, const babelwires::Context& context,
                     babelwires::UserLogger& userLogger);

} // namespace smf
