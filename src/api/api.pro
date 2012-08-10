#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T01:27:28
#
#-------------------------------------------------

QT       += network

TARGET = vkit
TEMPLATE = lib

DEFINES += VK_LIBRARY

SOURCES += client.cpp \
    connection.cpp \
    reply.cpp \
    json.cpp \
    longpoll.cpp \
    newsfeed.cpp \
    roster.cpp \
    message.cpp \
    messagemodel.cpp \
    chatsession.cpp \
    utils.cpp \
    dynamicpropertydata.cpp \
    wallsession.cpp \
    wallpost.cpp \
    contact.cpp \
    commentssession.cpp \
    audio.cpp \
    audioitem.cpp \
    abstractlistmodel.cpp \
    newsitem.cpp \
    attachment.cpp \
    photomanager.cpp \
    contentdownloader.cpp \
    groupmanager.cpp \
    groupchatsession.cpp \
    messagesession.cpp

HEADERS += client.h \
    vk_global.h \
    client_p.h \
    connection.h \
    reply.h \
    reply_p.h \
    json.h \
    longpoll.h \
    longpoll_p.h \
    newsfeed.h \
    roster.h \
    roster_p.h \
    message.h \
    messagemodel.h \
    chatsession.h \
    utils.h \
    dynamicpropertydata_p.h \
    wallsession.h \
    wallpost.h \
    contact.h \
    contact_p.h \
    commentssession.h \
    audio.h \
    audioitem.h \
    abstractlistmodel.h \
    newsitem.h \
    attachment.h \
    photomanager.h \
    contentdownloader.h \
    groupmanager.h \
    groupchatsession.h \
    messagesession.h \
    messagesession_p.h

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
    addFiles.sources = vkit.dll
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
