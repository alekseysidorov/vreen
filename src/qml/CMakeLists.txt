set(QT_USE_QTDECLARATIVE true)
set(VREEN_IMPORTS_DIR "${VREEN_IMPORTS_DIR}" CACHE INTERNAL "")

list(APPEND exta_src
	vreenplugin_qt4.cpp
)

add_qml_module(vreenplugin
    CXX11
    URI com.vk.api
    QML_DIR qmldir
    SOURCE_DIR src
    SOURCES "vreenplugin_qt4.cpp"    
    LIBRARIES vreen ${QT_QTDECLARATIVE_LIBRARY}
    INCLUDES ../api ${QT_QTDECLARATIVE_INCLUDE_DIR}
    IMPORTS_DIR ${VREEN_IMPORTS_DIR}
)
