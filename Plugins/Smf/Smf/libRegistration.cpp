/**
 * Registration of factories for the Standard MIDI File support.
 *
 * (C) 2021 Malcolm Tyrrell
 *
 * Licensed under the GPLv3.0. See LICENSE file.
 **/
#include <Smf/libRegistration.hpp>

#include <MusicLib/Percussion/builtInPercussionInstruments.hpp>

#include <BaseLib/Context/context.hpp>
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

void smf::registerLib(babelwires::Context& context) {
    babelwires::TypeSystem& typeSystem = context.get<babelwires::TypeSystem>();

    // Formats
    context.get<babelwires::SourceFileFormatRegistry>().addEntry(std::make_unique<SmfSourceFormat>());
    context.get<babelwires::TargetFileFormatRegistry>().addEntry(std::make_unique<SmfTargetFormat>());

    // Types
    typeSystem.addType<GMSpecType>();
    typeSystem.addType<MidiMetadata>(typeSystem);
    typeSystem.addType<MidiChannel>();
    typeSystem.addType<MidiTrackAndChannel>(typeSystem);
    typeSystem.addType<MidiTrackAndChannelArray>(typeSystem);
    typeSystem.addType<RecordOfMidiTracks>(typeSystem);
    typeSystem.addType<SmfSequence>(typeSystem);

    // Percussion types
    const auto& builtInPercussion = typeSystem.getRegisteredType<bw_music::BuiltInPercussionInstruments>();
    typeSystem.addType<GMPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2StandardPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2RoomPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2PowerPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2ElectronicPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2AnalogPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2JazzPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2BrushPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2OrchestraPercussionSet>(*builtInPercussion);
    typeSystem.addType<GM2SFXPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsStandard1PercussionSet>(*builtInPercussion);
    typeSystem.addType<GsRoomPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsPowerPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsElectronicPercussionSet>(*builtInPercussion);
    typeSystem.addType<Gs808909PercussionSet>(*builtInPercussion);
    typeSystem.addType<GsJazzPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsBrushPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsOrchestraPercussionSet>(*builtInPercussion);
    typeSystem.addType<GsSFXPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgStandard1PercussionSet>(*builtInPercussion);
    typeSystem.addType<XgRoomPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgRockPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgElectroPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgAnalogPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgJazzPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgBrushPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgClassicPercussionSet>(*builtInPercussion);
    typeSystem.addType<XgSFX1PercussionSet>(*builtInPercussion);
    typeSystem.addType<XgSFX2PercussionSet>(*builtInPercussion);
}
