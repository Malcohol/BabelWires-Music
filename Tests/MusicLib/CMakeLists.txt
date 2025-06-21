SET( SEQUENCELIB_TESTS_SRCS
      musicLibTests.cpp
      chordMapProcessorTest.cpp
      fingeredChordsProcessorTest.cpp
      concatenateProcessorTest.cpp
      excerptProcessorTest.cpp
      filteredTrackIteratorTest.cpp
      mergeProcessorTest.cpp
      monophonicNoteIteratorTest.cpp
      monophonicSubtracksProcessorTest.cpp
      musicTypesTest.cpp
      percussionMapProcessorTest.cpp
      percussionSetWithPitchMapTest.cpp
      quantizeProcessorTest.cpp
      repeatProcessorTest.cpp
      splitAtPitchProcessorTest.cpp
      trackBuilderTest.cpp
      trackTest.cpp
      trackTraverserTest.cpp
      trackTypeTest.cpp
      transposeProcessorTest.cpp
   )

ADD_EXECUTABLE( musicLibTests ${SEQUENCELIB_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( musicLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(musicLibTests Common musicLib testUtils libTestUtils seqTestUtils gtest)
