SET( MUSICLIB_SRCS
	Utilities/musicUtilities.cpp
	Processors/chordMapProcessor.cpp
	Processors/concatenateProcessor.cpp
	Processors/excerptProcessor.cpp
	Processors/fingeredChordsProcessor.cpp
	Processors/fitToChordsProcessor.cpp
	Processors/mergeProcessor.cpp
	Processors/monophonicSubtracksProcessor.cpp
	Processors/percussionMapProcessor.cpp
	Processors/quantizeProcessor.cpp
	Processors/repeatProcessor.cpp
	Processors/silenceProcessor.cpp
	Processors/splitAtPitchProcessor.cpp
	Processors/transposeProcessor.cpp
	Functions/appendTrackFunction.cpp
	Functions/fingeredChordsFunction.cpp
	Functions/fitToChordFunction.cpp
	Functions/mapChordsFunction.cpp
	Functions/excerptFunction.cpp
	Functions/repeatFunction.cpp
	Functions/percussionMapFunction.cpp
	Functions/mergeFunction.cpp
	Functions/monophonicSubtracksFunction.cpp
	Functions/quantizeFunction.cpp
	Functions/splitAtPitchFunction.cpp
	Functions/transposeFunction.cpp
	Types/Track/TrackEvents/chordEvents.cpp
	Types/Track/TrackEvents/noteEvents.cpp
	Types/Track/TrackEvents/percussionEvents.cpp
	Types/Track/TrackEvents/trackEvent.cpp
	Types/Track/track.cpp
	Types/Track/trackType.cpp
	Types/Track/trackTypeConstructor.cpp
	chord.cpp
	musicTypes.cpp
	Percussion/percussionTypeTag.cpp
	Percussion/builtInPercussionInstruments.cpp
	Percussion/percussionSetWithPitchMap.cpp
	pitch.cpp
	Utilities/monophonicNoteIterator.cpp
	Types/Track/trackBuilder.cpp
	Utilities/trackValidator.cpp
	libRegistration.cpp
   )

ADD_LIBRARY( musicLib ${MUSICLIB_SRCS} )
TARGET_INCLUDE_DIRECTORIES( musicLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../../.. )
TARGET_LINK_LIBRARIES(musicLib PUBLIC BabelWiresLib tinyxml2)
