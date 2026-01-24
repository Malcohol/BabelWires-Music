SET( MUSICLIBUI_SRCS
    ValueModels/trackValueModel.cpp
    libRegistration.cpp
)

ADD_LIBRARY( musicLibUi ${MUSICLIBUI_SRCS} )
TARGET_INCLUDE_DIRECTORIES( musicLibUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )
TARGET_LINK_LIBRARIES( musicLibUi BabelWiresQtUi BabelWiresLib musicLib BaseLib)