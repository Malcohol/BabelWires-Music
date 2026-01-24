SET( SMF_TESTS_SRCS
      percussionTests.cpp
      smfTests.cpp
      saveLoadTests.cpp
      testSuiteTests.cpp
   )

FILE(GLOB MidiTestSuiteFiles "${CMAKE_CURRENT_SOURCE_DIR}/test-midi-files/midi/*.mid")

MESSAGE("Copying test suite files to build folder")
FOREACH(file ${MidiTestSuiteFiles})
   GET_FILENAME_COMPONENT(fileName ${file} NAME)
   CONFIGURE_FILE(${file} "${CMAKE_CURRENT_BINARY_DIR}/${fileName}" COPYONLY)
ENDFOREACH()

ADD_EXECUTABLE( SmfTests ${SMF_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( SmfTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../.. )
TARGET_LINK_LIBRARIES(SmfTests BaseLib seqTestUtils SmfLib testUtils libTestUtils gtest)
