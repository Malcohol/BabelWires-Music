SET( TESTPLUGIN_SRCS
    simpleAccompaniment.cpp
    testTrackContainer.cpp
    libRegistration.cpp
   )

ADD_LIBRARY( TestPluginLib ${TESTPLUGIN_SRCS} )
TARGET_INCLUDE_DIRECTORIES( TestPluginLib
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Domains/Music/Plugins>
        $<INSTALL_INTERFACE:include>
)
TARGET_LINK_LIBRARIES(TestPluginLib PUBLIC BabelWiresLib musicLib)

if(BUILD_SHARED_LIBS)
    set_target_properties(TestPluginLib PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON)
    target_compile_definitions(TestPluginLib PRIVATE TESTPLUGINLIB_EXPORTS)
else()
    target_compile_definitions(TestPluginLib PUBLIC TESTPLUGINLIB_STATIC)
endif()