/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/libRegistration.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Smf/Percussion/gm2AnalogPercussionSet.hpp>
#include <Smf/Percussion/gm2BrushPercussionSet.hpp>
#include <Smf/Percussion/gm2ElectronicPercussionSet.hpp>
#include <Smf/Percussion/gm2JazzPercussionSet.hpp>
#include <Smf/Percussion/gm2OrchestraPercussionSet.hpp>
#include <Smf/Percussion/gm2PowerPercussionSet.hpp>
#include <Smf/Percussion/gm2RoomPercussionSet.hpp>
#include <Smf/Percussion/gm2SFXPercussionSet.hpp>
#include <Smf/Percussion/gm2StandardPercussionSet.hpp>
#include <Smf/Percussion/gmPercussionSet.hpp>
#include <Smf/Percussion/gs808909PercussionSet.hpp>
#include <Smf/Percussion/gsBrushPercussionSet.hpp>
#include <Smf/Percussion/gsElectronicPercussionSet.hpp>
#include <Smf/Percussion/gsJazzPercussionSet.hpp>
#include <Smf/Percussion/gsOrchestraPercussionSet.hpp>
#include <Smf/Percussion/gsPowerPercussionSet.hpp>
#include <Smf/Percussion/gsRoomPercussionSet.hpp>
#include <Smf/Percussion/gsSFXPercussionSet.hpp>
#include <Smf/Percussion/gsStandard1PercussionSet.hpp>
#include <Smf/Percussion/xgAnalogPercussionSet.hpp>
#include <Smf/Percussion/xgBrushPercussionSet.hpp>
#include <Smf/Percussion/xgClassicPercussionSet.hpp>
#include <Smf/Percussion/xgElectroPercussionSet.hpp>
#include <Smf/Percussion/xgJazzPercussionSet.hpp>
#include <Smf/Percussion/xgRockPercussionSet.hpp>
#include <Smf/Percussion/xgRoomPercussionSet.hpp>
#include <Smf/Percussion/xgSFX1PercussionSet.hpp>
#include <Smf/Percussion/xgSFX2PercussionSet.hpp>
#include <Smf/Percussion/xgStandard1PercussionSet.hpp>
#include <Smf/gmSpec.hpp>
#include <Smf/midiChannel.hpp>
#include <Smf/midiMetadata.hpp>
#include <Smf/midiTrackAndChannel.hpp>
#include <Smf/midiTrackAndChannelArray.hpp>
#include <Smf/recordOfMidiTracks.hpp>
#include <Smf/smfFormat.hpp>
#include <Smf/smfSequence.hpp>

void smf::registerLib(babelwires::ProjectContext& context) {
    // Formats
    context.m_sourceFileFormatReg.addEntry(std::make_unique<SmfSourceFormat>());
    context.m_targetFileFormatReg.addEntry(std::make_unique<SmfTargetFormat>());

    // Types
    context.m_typeSystem.addType<GMSpecType>();
    context.m_typeSystem.addType<MidiMetadata>(context.m_typeSystem);
    context.m_typeSystem.addType<MidiChannel>();
    context.m_typeSystem.addType<MidiTrackAndChannel>(context.m_typeSystem);
    context.m_typeSystem.addType<MidiTrackAndChannelArray>(context.m_typeSystem);
    context.m_typeSystem.addType<RecordOfMidiTracks>(context.m_typeSystem);
    context.m_typeSystem.addType<SmfSequence>(context.m_typeSystem);

    // Percussion types
    const auto& builtInPercussion = context.m_typeSystem.getRegisteredType<bw_music::BuiltInPercussionInstruments>();
    context.m_typeSystem.addType<GMPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2StandardPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2RoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2PowerPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2ElectronicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2AnalogPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2JazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2BrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2OrchestraPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GM2SFXPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsStandard1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsRoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsPowerPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsElectronicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<Gs808909PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsJazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsBrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsOrchestraPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<GsSFXPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgStandard1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgRoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgRockPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgElectroPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgAnalogPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgJazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgBrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgClassicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgSFX1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addType<XgSFX2PercussionSet>(*builtInPercussion);
}
