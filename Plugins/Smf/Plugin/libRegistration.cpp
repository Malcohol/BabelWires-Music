/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Plugins/Smf/Plugin/libRegistration.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>

#include <BabelWiresLib/Project/projectContext.hpp>
#include <BabelWiresLib/TypeSystem/typeSystem.hpp>

#include <Plugins/Smf/Plugin/Percussion/gm2AnalogPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2BrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2ElectronicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2JazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2OrchestraPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2PowerPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2RoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2SFXPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gm2StandardPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gmPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gs808909PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsBrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsElectronicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsJazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsOrchestraPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsPowerPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsRoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsSFXPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/gsStandard1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgAnalogPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgBrushPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgClassicPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgElectroPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgJazzPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgRockPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgRoomPercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgSFX1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgSFX2PercussionSet.hpp>
#include <Plugins/Smf/Plugin/Percussion/xgStandard1PercussionSet.hpp>
#include <Plugins/Smf/Plugin/gmSpec.hpp>
#include <Plugins/Smf/Plugin/midiChannel.hpp>
#include <Plugins/Smf/Plugin/midiMetadata.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannel.hpp>
#include <Plugins/Smf/Plugin/midiTrackAndChannelArray.hpp>
#include <Plugins/Smf/Plugin/recordOfMidiTracks.hpp>
#include <Plugins/Smf/Plugin/smfFormat.hpp>
#include <Plugins/Smf/Plugin/smfSequence.hpp>

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
