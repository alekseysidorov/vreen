#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T01:27:28
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = vk
TEMPLATE = lib

DEFINES += VK_LIBRARY

SOURCES += client.cpp \
    connection.cpp \
    reply.cpp \
    json.cpp \
    directconnection.cpp

HEADERS += client.h \
    vk_global.h \
    client_p.h \
    connection.h \
    reply.h \
    reply_p.h \
    json.h \
    directconnection_p.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7156367
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = api.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix {
    QMAKE_CXXFLAGS += -std=c++0x
}
#linux-g++ {
#    QMAKE_CXXFLAGS += -std=c++0x
#}
#linux-clang {
#    QMAKE_CXXFLAGS += -std=c++0x
#}

exists(../3rdparty/k8json) {
    include(../3rdparty/k8json/k8json.pri)
    DEFINES += K8JSON_INCLUDE_GENERATOR
    DEFINES += K8JSON_INCLUDE_COMPLEX_GENERATOR
} else {
    CONFIG += link_pkgconfig
    PKGCONFIG += k8json
}
