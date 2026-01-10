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
    context.m_typeSystem.addEntry<GMSpecType>();
    context.m_typeSystem.addEntry<MidiMetadata>(context.m_typeSystem);
    context.m_typeSystem.addEntry<MidiChannel>();
    context.m_typeSystem.addEntry<MidiTrackAndChannel>(context.m_typeSystem);
    context.m_typeSystem.addEntry<MidiTrackAndChannelArray>(context.m_typeSystem);
    context.m_typeSystem.addEntry<RecordOfMidiTracks>(context.m_typeSystem);
    context.m_typeSystem.addEntry<SmfSequence>(context.m_typeSystem);

    // Percussion types
    const auto& builtInPercussion = context.m_typeSystem.getRegisteredType<bw_music::BuiltInPercussionInstruments>();
    context.m_typeSystem.addEntry<GMPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2StandardPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2RoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2PowerPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2ElectronicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2AnalogPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2JazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2BrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2OrchestraPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GM2SFXPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsStandard1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsRoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsPowerPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsElectronicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<Gs808909PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsJazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsBrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsOrchestraPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<GsSFXPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgStandard1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgRoomPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgRockPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgElectroPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgAnalogPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgJazzPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgBrushPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgClassicPercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgSFX1PercussionSet>(*builtInPercussion);
    context.m_typeSystem.addEntry<XgSFX2PercussionSet>(*builtInPercussion);
}
