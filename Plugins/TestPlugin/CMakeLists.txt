SET( TESTPLUGIN_SRCS
    testTrackContainer.cpp
    libRegistration.cpp
   )

ADD_LIBRARY( TestPluginLib ${TESTPLUGIN_SRCS} )
TARGET_INCLUDE_DIRECTORIES( TestPluginLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../../.. )
TARGET_LINK_LIBRARIES(TestPluginLib PUBLIC BabelWiresLib musicLib)