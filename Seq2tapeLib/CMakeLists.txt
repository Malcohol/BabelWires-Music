SET( SEQ2TAPELIB_SRCS
	tapeFile.cpp
	tapeFileFormat.cpp
	waveReader.cpp
	sampleReader.cpp
	Audio/audioDest.cpp
	Audio/audioSource.cpp
	Audio/audioInterface.cpp
	Audio/FileAudio/fileAudioDest.cpp
	Audio/FileAudio/fileAudioSource.cpp
   )

ADD_LIBRARY( Seq2tapeLib ${SEQ2TAPELIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( Seq2tapeLib
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Domains/Music>
		$<INSTALL_INTERFACE:include>
)
TARGET_LINK_LIBRARIES(Seq2tapeLib BaseLib sndfile)

if(BUILD_SHARED_LIBS)
	set_target_properties(Seq2tapeLib PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN ON)
	target_compile_definitions(Seq2tapeLib PRIVATE SEQ2TAPELIB_EXPORTS)
else()
	target_compile_definitions(Seq2tapeLib PUBLIC SEQ2TAPELIB_STATIC)
endif()
