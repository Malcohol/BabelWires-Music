/**
 * Parse a Standard MIDI File into a tree of Features.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/smfParser.hpp>

#include <Smf/Parsing/smfByteParser.hpp>
#include <Smf/Percussion/gm2StandardPercussionSet.hpp>
#include <Smf/Percussion/gmPercussionSet.hpp>
#include <Smf/smfCommon.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>
#include <MusicLib/Types/Track/TrackEvents/noteEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/notePressureEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/panEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/percussionEvents.hpp>
#include <MusicLib/Types/Track/TrackEvents/pitchBendEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/pressureEvent.hpp>
#include <MusicLib/Types/Track/TrackEvents/tempoEvent.hpp>
#include <MusicLib/Types/Track/trackBuilder.hpp>
#include <MusicLib/Utilities/minCentreMaxValue.hpp>

#include <BabelWiresLib/TypeSystem/typeSystem.hpp>
#include <BabelWiresLib/Types/File/fileTypeT.hpp>
#include <BaseLib/Context/context.hpp>
#include <BaseLib/Math/fixed.hpp>

#include <BaseLib/Log/debugLogger.hpp>
#include <BaseLib/Result/result.hpp>
#include <BaseLib/Result/resultDSL.hpp>

#include <cassert>
#include <cmath>
#include <iomanip>

namespace {
    static const int MAX_CHANNELS = 16;

    // See page 237 of the SC-8850 English manual
    const std::array<unsigned int, 16> s_gsBlockToPartMapping{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16};

    std::uint16_t combine14BitControllerValue(babelwires::Byte msb, babelwires::Byte lsb) {
        return static_cast<std::uint16_t>((static_cast<std::uint16_t>(msb) << 7) | lsb);
    }
} // namespace

// ---------------------------------------------------------------------------
// SmfConsumer
// ---------------------------------------------------------------------------

smf::SmfConsumer::SmfConsumer(const babelwires::Context& context, babelwires::UserLogger& userLogger)
    : m_projectContext(context)
    , m_userLogger(userLogger)
    , m_standardPercussionSets(context) {

    m_result = std::make_unique<babelwires::ValueTreeRoot>(
        context.get<babelwires::TypeSystem>(),
        babelwires::FileTypeT<SmfSequence>::getType(context.get<babelwires::TypeSystem>()));
    m_result->setToDefault();
}

smf::SmfSequence::ConstInstance smf::SmfConsumer::getSmfSequenceConst() const {
    return babelwires::FileTypeT<SmfSequence>::ConstInstance(*m_result).getConts();
}

smf::SmfSequence::Instance smf::SmfConsumer::getSmfSequence() {
    return babelwires::FileTypeT<SmfSequence>::Instance(*m_result).getConts();
}

smf::MidiMetadata::Instance smf::SmfConsumer::getMidiMetadata() {
    return getSmfSequence().getMeta();
}

bw_music::ModelDuration smf::SmfConsumer::ticksToDuration(std::uint64_t ticks) const {
    return bw_music::ModelDuration(ticks, m_division * 4);
}

babelwires::Result smf::SmfConsumer::onSequenceStart(std::uint16_t numTracks, std::uint16_t format,
                                                     std::uint16_t division) {
    if (division & (1 << 15)) {
        return babelwires::Error() << "SMPTE format durations not supported";
    }
    if (format == 2) {
        return babelwires::Error() << "Standard MIDI File Format 2 files are not currently supported";
    }
    m_numTracks = numTracks;
    m_division = division;
    m_format = format;
    if (m_format == 1) {
        getSmfSequence().selectTag("SMF1");
    }
    return {};
}

std::unique_ptr<smf::SmfTrackEventConsumer> smf::SmfConsumer::onTrack(std::uint16_t trackIndex) {
    const bool hasMainMetadata = (m_format == 0) || (trackIndex == 0);
    return std::make_unique<TrackConsumer>(*this, trackIndex, hasMainMetadata);
}

babelwires::Result smf::SmfConsumer::readTempoEvent(int trackIndex, bw_music::ModelDuration absoluteTime,
                                                    std::uint32_t tempoValue) {
    ASSIGN_OR_ERROR(const auto tempo, bw_music::TempoValue::fromMicrosecondsPerQuaternote(tempoValue));

    if (auto existing = m_globalTempoEvents.find(absoluteTime); existing != m_globalTempoEvents.end()) {
        if (existing->second.m_trackIndex == trackIndex) {
            m_userLogger.logWarning() << "Multiple tempo events at the same tick in SMF track " << trackIndex
                                      << "; using the last event in stream order";
            existing->second.m_tempo = tempo;
        } else {
            // It isn't specified how to handle this, but higher-numbered tracks are often processed after
            // lower-numbered tracks, so this policy seems pragmatic.
            m_userLogger.logWarning()
                << "Conflicting simultaneous tempo events in multiple SMF1 tracks; using the higher-numbered track";
            if (existing->second.m_trackIndex > trackIndex) {
                return {};
            }
            existing->second = {trackIndex, tempo};
        }
    } else {
        m_globalTempoEvents.emplace(absoluteTime, NormalizedTempoEvent{trackIndex, tempo});
    }

    // TODO: Always activate the ITempo field, since 120 bpm should be assumed if no other tempo event is present.
    if (absoluteTime == 0) {
        const double roundedBpm = tempo.getBpmRounded(2);
        getMidiMetadata().activateAndGetITempo().set(babelwires::Fixed::assertFromDouble(roundedBpm, 2));
    }
    return {};
}

void smf::SmfConsumer::setGMSpec(GMSpecType::Value gmSpec) {
    for (int i = 0; i < 16; ++i) {
        m_channelState[i].m_kitIfPercussion = m_standardPercussionSets.getDefaultPercussionSet(gmSpec, i);
    }
    getMidiMetadata().getSpec().set(gmSpec);
}

void smf::SmfConsumer::setBankMSB(unsigned int channelNumber, const babelwires::Byte msbValue) {
    m_channelState[channelNumber].m_channelSetupInfo.m_bankMSB = msbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfConsumer::setBankLSB(unsigned int channelNumber, const babelwires::Byte lsbValue) {
    m_channelState[channelNumber].m_channelSetupInfo.m_bankLSB = lsbValue;
    onChangeProgram(channelNumber);
}

void smf::SmfConsumer::setProgram(unsigned int channelNumber, const babelwires::Byte value) {
    m_channelState[channelNumber].m_channelSetupInfo.m_program = value;
    onChangeProgram(channelNumber);
}

void smf::SmfConsumer::setGsPartMode(unsigned int blockNumber, babelwires::Byte value) {
    // For now, assume the midi channels for each part are unchanged.
    // I'm indexing midi channels from 0.
    const unsigned int channelNumber = s_gsBlockToPartMapping[blockNumber] - 1;
    m_channelState[channelNumber].m_channelSetupInfo.m_gsPartMode = value;
    onChangeProgram(channelNumber);
}

// Right now, just trying to determine which percussionSet is in use if any.
void smf::SmfConsumer::onChangeProgram(unsigned int channelNumber) {
    ChannelState& channelSetup = m_channelState[channelNumber];
    const GMSpecType::Value gmSpec = getMidiMetadata().getSpec().get();
    channelSetup.m_kitIfPercussion =
        m_standardPercussionSets.getPercussionSetFromChannelSetupInfo(gmSpec, channelSetup.m_channelSetupInfo);
}

void smf::SmfConsumer::interpretSysExForGMSpec(std::span<const std::uint8_t> data) {
    if (data.empty()) {
        return;
    }
    const babelwires::Byte headerId = data[0];
    if (headerId == 0x7E) {
        // Universal SysEx
        if (data.size() < 4) {
            return;
        }
        const babelwires::Byte subId1 = data[2];
        const babelwires::Byte subId2 = data[3];
        if (subId1 == 0x09) {
            // General MIDI message
            if (subId2 == 0x01) {
                // General MIDI On
                setGMSpec(GMSpecType::Value::GM);
                babelwires::logDebug() << "General MIDI On";
            } else if (subId2 == 0x02) {
                // General MIDI Off
                babelwires::logDebug() << "General MIDI Off";
            } else if (subId2 == 0x03) {
                // General MIDI 2 On
                setGMSpec(GMSpecType::Value::GM2);
                babelwires::logDebug() << "General MIDI 2 On";
            } else {
                babelwires::logDebug() << "Ignoring unrecognized General MIDI SysEx message";
            }
            if ((data.size() > 4) && (data[4] != 0xF7)) {
                m_userLogger.logWarning() << "Improperly terminated General MIDI SysEx message";
            }
        }
    } else if (headerId == 0x41) {
        // Roland SysEx
        const unsigned int messageSize = data.size();
        if (messageSize < 3) {
            return;
        }
        if (data[messageSize - 1] != 0xF7) {
            m_userLogger.logWarning() << "Improperly terminated Roland SysEx message";
        }
        // Checksum
        babelwires::Byte checkSum = 0;
        for (unsigned int i = 4; i < messageSize - 2; ++i) {
            // This can overflow without problems.
            checkSum += data[i];
        }
        if (data[messageSize - 2] != ((0x80 - (checkSum % 0x80)) % 0x80)) {
            m_userLogger.logWarning() << "Ignoring Roland SysEx message with invalid checksum";
            return;
        }
        const auto matches = [&data](std::initializer_list<std::int16_t> message) {
            if (data.size() != message.size()) {
                return false;
            }
            int i = 0;
            for (const std::int16_t expected : message) {
                if ((expected != -1) && (data[i] != expected)) {
                    return false;
                }
                ++i;
            }
            return true;
        };
        if (matches({0x41, -1, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7})) {
            setGMSpec(GMSpecType::Value::GS);
            babelwires::logDebug() << "Roland GS Reset";
            return;
        }
        if (matches({0x41, -1, 0x42, 0x12, 0x40, -1, 0x15, -1, -1, 0xF7}) && ((data[5] & 0xf0) == 0x10)) {
            // Use for rhythm part.
            const babelwires::Byte blockNumber = data[5] & 0x0f;
            const babelwires::Byte value = data[7];
            if (value > 2) {
                m_userLogger.logWarning()
                    << "Ignoring Roland SysEx use for rhythm part message with out of range value";
            } else {
                setGsPartMode(blockNumber, value);
            }
            return;
        }
    } else if (headerId == 0x43) {
        // Yamaha SysEx
        if (data.size() == 8 && data[2] == 0x4C && data[3] == 0x00 && data[4] == 0x00 && data[5] == 0x7E &&
            data[6] == 0x00 && data[7] == 0xF7) {
            setGMSpec(GMSpecType::Value::XG);
            babelwires::logDebug() << "Yamaha XG Reset";
            return;
        }
    }
}

babelwires::Result smf::SmfConsumer::buildOutputTracks() {
    if (m_format == 0) {
        // Format 0: a single multi-channel track, split per channel.
        auto tracks = getSmfSequence().getTrcks0();
        // m_normalizedTracks holds at most one entry for format 0 (track index 0), with
        // m_track being the privileged channel and m_extraTracks the rest. We need to
        // reassemble all channels keyed by channel number instead.
        if (m_normalizedTracks.empty()) {
            return {};
        }
        Format1TrackData& data = m_normalizedTracks.begin()->second;
        tracks.activateAndGetTrack(data.m_channelNumber).set(std::move(data.m_track));
        for (auto& [channelNumber, extraTrack] : data.m_extraTracks) {
            tracks.activateAndGetTrack(channelNumber).set(std::move(extraTrack));
        }
        return {};
    } else {
        // Format 1.
        auto tracks = getSmfSequence().getTrcks1();
        tracks.setSize(std::max(1, static_cast<int>(m_normalizedTracks.size())));
        int i = 0;
        for (auto& entry : m_normalizedTracks) {
            Format1TrackData& data = entry.second;
            auto track = tracks.getEntry(i);
            track.getChan().set(data.m_channelNumber);
            track.getTrack().set(std::move(data.m_track));
            for (auto& [channelNumber, extraTrack] : data.m_extraTracks) {
                track.activateAndGetTrack(channelNumber).set(std::move(extraTrack));
            }
            ++i;
        }
        return {};
    }
}

babelwires::Result smf::SmfConsumer::finalize() {
    // Assemble the global tempo track.
    if (!m_globalTempoEvents.empty()) {
        bw_music::TrackBuilder globalTrack;
        bw_music::ModelDuration timeOfLastEvent = 0;
        for (const auto& [absoluteTime, tempo] : m_globalTempoEvents) {
            globalTrack.addEvent(bw_music::TempoEvent{absoluteTime - timeOfLastEvent, tempo.m_tempo});
            timeOfLastEvent = absoluteTime;
        }
        getSmfSequence().getGlobal().set(globalTrack.finishAndGetTrack());
    }
    return buildOutputTracks();
}

// ---------------------------------------------------------------------------
// SmfConsumer::TrackConsumer
// ---------------------------------------------------------------------------

smf::SmfConsumer::TrackConsumer::TrackConsumer(SmfConsumer& owner, std::uint16_t trackIndex, bool hasMainMetadata)
    : m_owner(owner)
    , m_trackIndex(trackIndex)
    , m_hasMainMetadata(hasMainMetadata)
    , m_channels{} {}

smf::SmfConsumer::TrackConsumer::PerChannelInfo*
smf::SmfConsumer::TrackConsumer::getChannel(unsigned int channelNumber) {
    assert(0 <= channelNumber);
    assert(channelNumber < MAX_CHANNELS);
    auto& channel = m_channels[channelNumber];
    if (channel == nullptr) {
        channel = std::make_unique<PerChannelInfo>();
    }
    return channel.get();
}

bw_music::ModelDuration smf::SmfConsumer::TrackConsumer::timeSinceLastHandledEvent(TimeInfo timeInfo) const {
    return m_owner.ticksToDuration(timeInfo.m_ticksSinceLastHandledEvent);
}

bool smf::SmfConsumer::TrackConsumer::addNoteOn(unsigned int channelNumber,
                                                bw_music::ModelDuration timeSinceLastTrackEvent, bw_music::Pitch pitch,
                                                bw_music::VelocityStorage velocity) {
    if (const bw_music::PercussionSetWithPitchMap* const percussionSet =
            m_owner.m_channelState[channelNumber].m_kitIfPercussion) {
        if (auto instrument = percussionSet->tryGetInstrumentFromPitch(pitch)) {
            return addEvent<bw_music::PercussionOnEvent>(channelNumber, timeSinceLastTrackEvent, *instrument, velocity);
        }
        return false;
    } else {
        return addEvent<bw_music::NoteOnEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
    }
}

bool smf::SmfConsumer::TrackConsumer::addNoteOff(unsigned int channelNumber,
                                                 bw_music::ModelDuration timeSinceLastTrackEvent, bw_music::Pitch pitch,
                                                 bw_music::VelocityStorage velocity) {
    if (const bw_music::PercussionSetWithPitchMap* const percussionSet =
            m_owner.m_channelState[channelNumber].m_kitIfPercussion) {
        if (auto instrument = percussionSet->tryGetInstrumentFromPitch(pitch)) {
            return addEvent<bw_music::PercussionOffEvent>(channelNumber, timeSinceLastTrackEvent, *instrument,
                                                          velocity);
        }
        return false;
    } else {
        return addEvent<bw_music::NoteOffEvent>(channelNumber, timeSinceLastTrackEvent, pitch, velocity);
    }
}

bool smf::SmfConsumer::TrackConsumer::addEvent(unsigned int channelNumber, bw_music::TrackEvent&& event) {
    PerChannelInfo* channel = getChannel(channelNumber);
    m_timeSinceStart += event.getTimeSinceLastEvent();
    event.setTimeSinceLastEvent(m_timeSinceStart - channel->m_timeOfLastEvent);
    channel->m_track.addEvent(std::move(event));
    channel->m_timeOfLastEvent = m_timeSinceStart;
    return true;
}

void smf::SmfConsumer::TrackConsumer::setDurationsForAllChannels(bw_music::ModelDuration timeToEndOfTrackEvent) {
    const bw_music::ModelDuration duration = m_timeSinceStart + timeToEndOfTrackEvent;
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        if (m_channels[channelNumber] != nullptr) {
            m_channels[channelNumber]->m_trackDuration = duration;
        }
    }
}

template <typename STORAGE>
babelwires::ResultT<std::optional<STORAGE>>
smf::SmfConsumer::ChannelState::ControllerState::updateWithNewValue(babelwires::Byte value, bool isLsb) {
    if (isLsb) {
        m_lsb = value;
        if (m_msb.has_value()) {
            ASSIGN_OR_ERROR(const STORAGE fineStorage,
                            STORAGE::template fromUnsigned<14>(combine14BitControllerValue(*m_msb, *m_lsb)));
            return std::optional<STORAGE>{fineStorage};
        } else {
            // We don't have a complete 14-bit value yet, so no event is returned.
            return std::optional<STORAGE>{};
        }
    } else {
        m_msb = value;
        if (m_lsb.has_value()) {
            ASSIGN_OR_ERROR(const STORAGE fineStorage,
                            STORAGE::template fromUnsigned<14>(combine14BitControllerValue(*m_msb, *m_lsb)));
            return std::optional<STORAGE>{fineStorage};
        } else {
            // Assume a coarse/legacy 7-bit event (so, for example, 0x7f gets treated the same as 0x3fff).
            // If a corresponding LSB event follows at the same time, this event will be subsumed by the subsequent
            // 14-bit event when both events are processed by the TrackBuilder.
            ASSIGN_OR_ERROR(const STORAGE coarseStorage, STORAGE::template fromUnsigned<7>(value));
            return std::optional<STORAGE>{coarseStorage};
        }
    }
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onNoteOn(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t pitch7,
                                          std::uint8_t velocity7) {
    ASSIGN_OR_ERROR(const bw_music::VelocityStorage velocity, bw_music::MinMaxValue16::fromUnsigned<7>(velocity7));
    if (addNoteOn(channel4, timeSinceLastHandledEvent(timeInfo), pitch7, velocity)) {
        return EventHandlingResult::ResetTime;
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onNoteOff(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t pitch7,
                                           std::uint8_t velocity7) {
    ASSIGN_OR_ERROR(const bw_music::VelocityStorage velocity, bw_music::MinMaxValue16::fromUnsigned<7>(velocity7));
    if (addNoteOff(channel4, timeSinceLastHandledEvent(timeInfo), pitch7, velocity)) {
        return EventHandlingResult::ResetTime;
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onPolyphonicAftertouch(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t pitch7,
                                                        std::uint8_t pressure7) {
    ASSIGN_OR_ERROR(const bw_music::ControllerStorage pressure,
                    bw_music::ControllerStorage::fromUnsigned<7>(pressure7));
    if (addEvent<bw_music::NotePressureEvent>(channel4, timeSinceLastHandledEvent(timeInfo), pitch7, pressure)) {
        return EventHandlingResult::ResetTime;
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onChannelPressure(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t pressure7) {
    ASSIGN_OR_ERROR(const bw_music::ControllerStorage pressure,
                    bw_music::ControllerStorage::fromUnsigned<7>(pressure7));
    if (addEvent<bw_music::PressureEvent>(channel4, timeSinceLastHandledEvent(timeInfo), pressure)) {
        return EventHandlingResult::ResetTime;
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onPitchBend(TimeInfo timeInfo, std::uint8_t channel4, std::uint16_t value14) {
    ASSIGN_OR_ERROR(bw_music::CentredControllerStorage pitchBend,
                    bw_music::CentredControllerStorage::fromUnsigned<14>(value14));
    if (addEvent<bw_music::PitchBendEvent>(channel4, timeSinceLastHandledEvent(timeInfo), std::move(pitchBend))) {
        return EventHandlingResult::ResetTime;
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onControlChange(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t controller7,
                                                 std::uint8_t value7) {
    const bw_music::ModelDuration time = timeSinceLastHandledEvent(timeInfo);
    ChannelState& channelState = m_owner.m_channelState[channel4];

    switch (controller7) {
        case c_bankSelectMsbController:
            m_owner.setBankMSB(channel4, value7);
            return EventHandlingResult::AccumulateTime;
        case c_volumeMsbController: {
            ASSIGN_OR_ERROR(auto volume,
                            channelState.m_volume.updateWithNewValue<bw_music::ControllerStorage>(value7, false));
            return addEvent<bw_music::VolumeEvent>(channel4, time, *volume) ? EventHandlingResult::ResetTime
                                                                            : EventHandlingResult::AccumulateTime;
        }
        case c_volumeLsbController: {
            ASSIGN_OR_ERROR(auto volume,
                            channelState.m_volume.updateWithNewValue<bw_music::ControllerStorage>(value7, true));
            if (!volume.has_value()) {
                return EventHandlingResult::AccumulateTime;
            }
            return addEvent<bw_music::VolumeEvent>(channel4, time, *volume) ? EventHandlingResult::ResetTime
                                                                            : EventHandlingResult::AccumulateTime;
        }
        case c_panMsbController: {
            ASSIGN_OR_ERROR(auto pan,
                            channelState.m_pan.updateWithNewValue<bw_music::CentredControllerStorage>(value7, false));
            return addEvent<bw_music::PanEvent>(channel4, time, *pan) ? EventHandlingResult::ResetTime
                                                                      : EventHandlingResult::AccumulateTime;
        }
        case c_panLsbController: {
            ASSIGN_OR_ERROR(auto pan,
                            channelState.m_pan.updateWithNewValue<bw_music::CentredControllerStorage>(value7, true));
            if (!pan.has_value()) {
                return EventHandlingResult::AccumulateTime;
            }
            return addEvent<bw_music::PanEvent>(channel4, time, *pan) ? EventHandlingResult::ResetTime
                                                                      : EventHandlingResult::AccumulateTime;
        }
        case c_expressionMsbController: {
            ASSIGN_OR_ERROR(auto expression,
                            channelState.m_expression.updateWithNewValue<bw_music::ControllerStorage>(value7, false));
            return addEvent<bw_music::ExpressionEvent>(channel4, time, *expression) ? EventHandlingResult::ResetTime
                                                                                    : EventHandlingResult::AccumulateTime;
        }
        case c_expressionLsbController: {
            ASSIGN_OR_ERROR(auto expression,
                            channelState.m_expression.updateWithNewValue<bw_music::ControllerStorage>(value7, true));
            if (!expression.has_value()) {
                return EventHandlingResult::AccumulateTime;
            }
            return addEvent<bw_music::ExpressionEvent>(channel4, time, *expression) ? EventHandlingResult::ResetTime
                                                                                    : EventHandlingResult::AccumulateTime;
        }
        case c_bankSelectLsbController:
            m_owner.setBankLSB(channel4, value7);
            return EventHandlingResult::AccumulateTime;
        case c_sustainController: {
            ASSIGN_OR_ERROR(const bw_music::ControllerStorage sustain,
                            bw_music::ControllerStorage::fromUnsigned<7>(value7));
            return addEvent<bw_music::SustainEvent>(channel4, time, sustain) ? EventHandlingResult::ResetTime
                                                                             : EventHandlingResult::AccumulateTime;
        }
        default:
            return EventHandlingResult::AccumulateTime;
    }
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onProgramChange(TimeInfo timeInfo, std::uint8_t channel4, std::uint8_t program7) {
    m_owner.setProgram(channel4, program7);
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onTempoEvent(TimeInfo timeInfo, std::uint32_t tempoValue24) {
    DO_OR_ERROR(
        m_owner.readTempoEvent(m_trackIndex, m_owner.ticksToDuration(timeInfo.m_ticksSinceTrackStart), tempoValue24));
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onCopyright(TimeInfo timeInfo, std::span<const std::uint8_t> copyright) {
    if (m_hasMainMetadata) {
        std::string text(copyright.begin(), copyright.end());
        m_owner.getMidiMetadata().activateAndGetCopyR().set(babelwires::Text::tryFromPrintableAscii(text));
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onSequenceOrTrackName(TimeInfo timeInfo, std::span<const std::uint8_t> name) {
    if (m_hasMainMetadata) {
        std::string text(name.begin(), name.end());
        m_owner.getMidiMetadata().activateAndGetName().set(babelwires::Text::tryFromPrintableAscii(text));
    }
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onSysExEvent(TimeInfo timeInfo, std::span<const std::uint8_t> data) {
    m_owner.interpretSysExForGMSpec(data);
    return EventHandlingResult::AccumulateTime;
}

babelwires::ResultT<smf::SmfTrackEventConsumer::EventHandlingResult>
smf::SmfConsumer::TrackConsumer::onEndOfTrack(TimeInfo timeInfo) {
    setDurationsForAllChannels(timeSinceLastHandledEvent(timeInfo));
    finalizeTrack();
    return EventHandlingResult::AccumulateTime;
}

void smf::SmfConsumer::TrackConsumer::finalizeTrack() {
    if (m_finalized) {
        return;
    }
    m_finalized = true;

    // Convert the per-channel builders into actual tracks.
    std::array<bw_music::Track, 16> tracks;

    // If this is a format 1 track with multiple channels (rare but possible), privilege the
    // channel with the most events. For format 0, the same logic applies but all channels
    // are written to the output (see buildOutputTracks).
    int privilegedTrack = -1;
    int maxNumEvents = 0;
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        if (m_channels[channelNumber] != nullptr) {
            PerChannelInfo& info = *m_channels[channelNumber];
            if (m_owner.m_format == 0 && info.m_trackDuration > 0) {
                // Format 0: all channels share the MIDI track's duration. m_trackDuration is only
                // set if an end-of-track event was reached; a track which ended early (or which the
                // consumer finished early) leaves it 0, in which case we use the event duration.
                tracks[channelNumber] = info.m_track.finishAndGetTrack(info.m_trackDuration);
            } else {
                tracks[channelNumber] = info.m_track.finishAndGetTrack();
            }
            if (tracks[channelNumber].getNumEvents() > maxNumEvents) {
                privilegedTrack = channelNumber;
                maxNumEvents = tracks[channelNumber].getNumEvents();
            }
        }
    }

    if (privilegedTrack < 0) {
        // No channels had any events (after global events such as tempo were removed).
        return;
    }

    Format1TrackData result{static_cast<unsigned int>(privilegedTrack), std::move(tracks[privilegedTrack]), {}};
    for (int channelNumber = 0; channelNumber < MAX_CHANNELS; ++channelNumber) {
        if ((channelNumber != privilegedTrack) && (m_channels[channelNumber] != nullptr)) {
            result.m_extraTracks.emplace_back(channelNumber, std::move(tracks[channelNumber]));
        }
    }
    m_owner.m_normalizedTracks.emplace(m_trackIndex, std::move(result));
}

smf::SmfConsumer::TrackConsumer::~TrackConsumer() {
    // Finalization normally happens in onEndOfTrack. This is clean-up for tracks which
    // ended without an end-of-track event (i.e. the consumer signalled Done, or parsing
    // of a later track failed).
    finalizeTrack();
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

babelwires::ResultT<std::unique_ptr<babelwires::ValueTreeRoot>>
smf::parseSmfSequence(babelwires::DataSource& dataSource, const babelwires::Context& context,
                      babelwires::UserLogger& userLogger) {
    SmfConsumer consumer(context, userLogger);
    SmfByteParser parser(dataSource, consumer, userLogger);
    DO_OR_ERROR(parser.parse());
    DO_OR_ERROR(consumer.finalize());
    return consumer.getResult();
}
