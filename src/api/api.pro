#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T01:27:28
#
#-------------------------------------------------

QT       += network

TARGET = vreen
TEMPLATE = lib

DEFINES += VK_LIBRARY

SOURCES += client.cpp \
    utils.cpp \
    roster.cpp \
    reply.cpp \
    photomanager.cpp \
    newsitem.cpp \
    newsfeed.cpp \
    messagesession.cpp \
    messagemodel.cpp \
    message.cpp \
    longpoll.cpp \
    json.cpp \
    groupmanager.cpp \
    groupchatsession.cpp \
    dynamicpropertydata.cpp \
    contentdownloader.cpp \
    contact.cpp \
    connection.cpp \
    commentssession.cpp \
    chatsession.cpp \
    audioitem.cpp \
    audio.cpp \
    attachment.cpp \
    abstractlistmodel.cpp \
    wallsession.cpp \
    wallpost.cpp

HEADERS += client.h \
    vk_global.h \
    utils.h \
    roster_p.h \
    roster.h \
    reply_p.h \
    reply.h \
    photomanager.h \
    newsitem.h \
    newsfeed.h \
    messagesession_p.h \
    messagesession.h \
    messagemodel.h \
    message.h \
    longpoll_p.h \
    longpoll.h \
    json.h \
    groupmanager.h \
    groupchatsession.h \
    dynamicpropertydata_p.h \
    contentdownloader.h \
    contact_p.h \
    contact.h \
    connection.h \
    commentssession.h \
    client_p.h \
    chatsession.h \
    audioitem.h \
    audio.h \
    attachment.h \
    abstractlistmodel.h \
    wallsession.h \
    wallpost.h

PUBLIC_HEADERS = $$HEADERS
    
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
} else macx {
	CONFIG(debug, debug|release) {
		TARGET = $$member(TARGET, 0)_debug
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

OTHER_FILES += \
    CMakeLists.txt
