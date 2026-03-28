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
TARGET_INCLUDE_DIRECTORIES( Seq2tapeLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES(Seq2tapeLib BaseLib sndfile)

if(BUILD_SHARED_LIBS)
	set_target_properties(Seq2tapeLib PROPERTIES
		CXX_VISIBILITY_PRESET hidden
		VISIBILITY_INLINES_HIDDEN ON)
	target_compile_definitions(Seq2tapeLib PRIVATE SEQ2TAPELIB_EXPORTS)
endif()
