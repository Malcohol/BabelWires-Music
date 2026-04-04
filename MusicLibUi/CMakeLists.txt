SET( MUSICLIBUI_SRCS
    ValueModels/trackValueModel.cpp
    libRegistration.cpp
)

ADD_LIBRARY( musicLibUi ${MUSICLIBUI_SRCS} )
TARGET_INCLUDE_DIRECTORIES( musicLibUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( musicLibUi BabelWiresQtUi BabelWiresLib musicLib BaseLib)

if(BUILD_SHARED_LIBS)
    set_target_properties(musicLibUi PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON)
    target_compile_definitions(musicLibUi PRIVATE MUSICLIBUI_EXPORTS)
else()
    target_compile_definitions(musicLibUi PUBLIC MUSICLIBUI_STATIC)
endif()