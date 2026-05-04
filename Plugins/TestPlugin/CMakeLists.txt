SET( TESTPLUGIN_SRCS
    simpleAccompaniment.cpp
    testTrackContainer.cpp
    libRegistration.cpp
   )

babelwires_add_plugin(TestPlugin TestPluginLib
	SOURCES ${TESTPLUGIN_SRCS}
    ENTRY_SOURCE pluginEntry.cpp
    DEPS BabelWiresLib musicLib BaseLib
    INCLUDE_DIRS
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Domains/Music/Plugins>
        $<INSTALL_INTERFACE:include>
)
