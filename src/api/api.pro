#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T01:27:28
#
#-------------------------------------------------

include(../../vreen.pri)

QT       += network

TARGET = vreen
TEMPLATE = lib

DEFINES += VK_LIBRARY

SOURCES += $$PWD/*.cpp

PUBLIC_HEADERS += $$PWD/*[^p].h
PRIVATE_HEADERS += $$PWD/*_p.h
HEADERS = $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
    
exists(../3rdparty/k8json) {
    include(../3rdparty/k8json/k8json.pri)
    DEFINES += K8JSON_INCLUDE_GENERATOR
    DEFINES += K8JSON_INCLUDE_COMPLEX_GENERATOR
    INCLUDEPATH += ../3rdparty
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += k8json
}

isEmpty( PREFIX ):INSTALL_PREFIX = /usr
else:INSTALL_PREFIX = $${PREFIX}

VK_INSTALL_HEADERS = $${INSTALL_PREFIX}/include
contains(QMAKE_HOST.arch, x86_64) {
	VK_INSTALL_LIBS = $${INSTALL_PREFIX}/lib64
} else {
	VK_INSTALL_LIBS = $${INSTALL_PREFIX}/lib
}
#VK_INSTALL_LIBS = $$[QT_INSTALL_LIBS]

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7156367
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = vreen.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

win32 {
    CONFIG(debug, debug|release) {
	    TARGET = $$member(TARGET, 0)d
    }
} else {
        macx {
        CONFIG(debug, debug|release) {
            TARGET = $$member(TARGET, 0)_debug
        }
    }
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else:!isEmpty(MEEGO_VERSION_MAJOR) {
        target.path = /opt/nonameIM/lib
    } else {
        target.path = $$VK_INSTALL_LIBS

        installHeaders.files = $$PUBLIC_HEADERS
        installHeaders.path = $$VK_INSTALL_HEADERS/vk
        INSTALLS += installHeaders
    }
    INSTALLS += target
}

unix {
    QMAKE_CXXFLAGS += -std=c++0x \
        -fvisibility=hidden \
        -Wall -Wextra \
        -Wno-cast-align \
        -O2 -finline-functions
}

QMAKE_POST_LINK += $(MKDIR) $$VREEN_INCLUDE_DIR/private $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $(COPY) $$PWD/*.h $$VREEN_INCLUDE_DIR $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $(COPY) $$PWD/*_p.h $$VREEN_INCLUDE_DIR/private $$escape_expand(\\n\\t)
