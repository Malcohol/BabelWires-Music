SET( SMFLIB_SRCS
	libRegistration.cpp
	gmSpec.cpp
	midiChannel.cpp
	midiMetadata.cpp
	midiTrackAndChannel.cpp
	midiTrackAndChannelArray.cpp
	Percussion/gmPercussionSet.cpp
	Percussion/gm2AnalogPercussionSet.cpp
	Percussion/gm2BrushPercussionSet.cpp
	Percussion/gm2ElectronicPercussionSet.cpp
	Percussion/gm2JazzPercussionSet.cpp
	Percussion/gm2OrchestraPercussionSet.cpp
	Percussion/gm2PowerPercussionSet.cpp
	Percussion/gm2SFXPercussionSet.cpp
	Percussion/gm2StandardPercussionSet.cpp
	Percussion/gm2RoomPercussionSet.cpp
	Percussion/gs808909PercussionSet.cpp
	Percussion/gsBrushPercussionSet.cpp
	Percussion/gsElectronicPercussionSet.cpp
	Percussion/gsJazzPercussionSet.cpp
	Percussion/gsOrchestraPercussionSet.cpp
	Percussion/gsPowerPercussionSet.cpp	
	Percussion/gsRoomPercussionSet.cpp
	Percussion/gsStandard1PercussionSet.cpp
	Percussion/gsSFXPercussionSet.cpp
	Percussion/standardPercussionSets.cpp
	Percussion/xgAnalogPercussionSet.cpp
	Percussion/xgBrushPercussionSet.cpp
	Percussion/xgClassicPercussionSet.cpp
	Percussion/xgElectroPercussionSet.cpp
	Percussion/xgJazzPercussionSet.cpp
	Percussion/xgRockPercussionSet.cpp
	Percussion/xgRoomPercussionSet.cpp
	Percussion/xgSFX1PercussionSet.cpp
	Percussion/xgSFX2PercussionSet.cpp
	Percussion/xgStandard1PercussionSet.cpp
	recordOfMidiTracks.cpp
	smfFormat.cpp
	smfParser.cpp
	smfSequence.cpp
	smfWriter.cpp
	)

ADD_LIBRARY( SmfLib ${SMFLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SmfLib
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Domains/Music/Plugins>
		$<INSTALL_INTERFACE:include>
)
TARGET_LINK_LIBRARIES(SmfLib BabelWiresLib musicLib BaseLib)

if(BUILD_SHARED_LIBS)
	set_target_properties(SmfLib PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN ON)
	target_compile_definitions(SmfLib PRIVATE SMFLIB_EXPORTS)
else()
	target_compile_definitions(SmfLib PUBLIC SMFLIB_STATIC)
endif()