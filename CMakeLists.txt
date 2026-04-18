# Directory-scope override: plugins created under this domain go to plugins/Music.
set(BABELWIRES_PLUGIN_OUTPUT_DIR "${BABELWIRES_PLUGIN_OUTPUT_DIR}/Music")

ADD_SUBDIRECTORY( MusicLib )
ADD_SUBDIRECTORY( Seq2tapeLib )
ADD_SUBDIRECTORY( MusicLibUi )

ADD_SUBDIRECTORY( Plugins/Smf/Smf )
ADD_SUBDIRECTORY( Plugins/TestPlugin)
ADD_SUBDIRECTORY( Plugins/Smf/Tests )

ADD_SUBDIRECTORY( Seq2tapeExe )

ADD_SUBDIRECTORY( Tests/TestUtils )
ADD_SUBDIRECTORY( Tests/MusicLib )
ADD_SUBDIRECTORY( Tests/Seq2tapeLib )
