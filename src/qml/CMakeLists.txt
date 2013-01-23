set(QT_USE_QTDECLARATIVE true)
set(VREEN_IMPORTS_DIR "${VREEN_IMPORTS_DIR}" CACHE INTERNAL "")

if(USE_QT5)
    set(_extra_qt Quick)
    list(APPEND EXTRA_SRC
	vreenplugin_qt5.cpp
    )
else()
    set(_extra_qt Declarative)
    list(APPEND EXTRA_SRC
	vreenplugin_qt4.cpp
    )
endif()

if(VREEN_WITH_OAUTH)
    list(APPEND VREEN_PLUGIN_DEFINES "VREEN_WITH_OAUTH")
    list(APPEND VREEN_PLUGIN_LIBS vreenoauth)
    list(APPEND _extra_qt WebKit)
endif()

add_qml_module(vreenplugin
    CXX11
    ${DEVELOPER_FLAG}
    URI com.vk.api
    QML_DIR qmldir
    SOURCE_DIR src
    SOURCE_FILES ${EXTRA_SRC}
    LIBRARIES vreen ${VREEN_PLUGIN_LIBS}
    DEFINES ${VREEN_PLUGIN_DEFINES}
    INCLUDES ../api ${CMAKE_BINARY_DIR}/include ${CMAKE_BINARY_DIR}/include/vreen/${CMAKE_VREEN_VERSION_STRING}/
    IMPORTS_DIR ${VREEN_IMPORTS_DIR}
    QT Core Network Gui Widgets ${_extra_qt}
)
