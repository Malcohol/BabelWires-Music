SET( SEQ2TAPELIB_TESTS_SRCS
      audioInterfaceRegistryTest.cpp
      seq2tapeLibTests.cpp
      waveReaderTest.cpp
   )

ADD_EXECUTABLE( seq2tapeLibTests ${SEQ2TAPELIB_TESTS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seq2tapeLibTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(seq2tapeLibTests BaseLib Seq2tapeLib testUtils gtest)
