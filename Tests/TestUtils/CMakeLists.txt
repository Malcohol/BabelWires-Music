SET( SEQTESTUTILS_SRCS
      seqTestUtils.cpp
      testTrackContainer.cpp
   )

ADD_LIBRARY( seqTestUtils ${SEQTESTUTILS_SRCS} )
TARGET_INCLUDE_DIRECTORIES( seqTestUtils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../.. ${CMAKE_CURRENT_SOURCE_DIR}/../.. )
TARGET_LINK_LIBRARIES(seqTestUtils Common musicLib gtest)
