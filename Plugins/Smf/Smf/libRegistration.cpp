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
    // Formats
    context.getService<babelwires::SourceFileFormatRegistry>().addEntry(std::make_unique<SmfSourceFormat>());
    context.getService<babelwires::TargetFileFormatRegistry>().addEntry(std::make_unique<SmfTargetFormat>());

    // Types
    context.getService<babelwires::TypeSystem>().addType<GMSpecType>();
    context.getService<babelwires::TypeSystem>().addType<MidiMetadata>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<MidiChannel>();
    context.getService<babelwires::TypeSystem>().addType<MidiTrackAndChannel>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<MidiTrackAndChannelArray>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<RecordOfMidiTracks>(context.getService<babelwires::TypeSystem>());
    context.getService<babelwires::TypeSystem>().addType<SmfSequence>(context.getService<babelwires::TypeSystem>());

    // Percussion types
    const auto& builtInPercussion = context.getService<babelwires::TypeSystem>().getRegisteredType<bw_music::BuiltInPercussionInstruments>();
    context.getService<babelwires::TypeSystem>().addType<GMPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2StandardPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2RoomPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2PowerPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2ElectronicPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2AnalogPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2JazzPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2BrushPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2OrchestraPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GM2SFXPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsStandard1PercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsRoomPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsPowerPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsElectronicPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<Gs808909PercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsJazzPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsBrushPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsOrchestraPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<GsSFXPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgStandard1PercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgRoomPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgRockPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgElectroPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgAnalogPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgJazzPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgBrushPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgClassicPercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgSFX1PercussionSet>(*builtInPercussion);
    context.getService<babelwires::TypeSystem>().addType<XgSFX2PercussionSet>(*builtInPercussion);
}
