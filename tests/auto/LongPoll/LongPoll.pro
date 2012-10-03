#-------------------------------------------------
#
# Project created by QtCreator 2012-03-23T15:29:56
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_longpolltest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_longpolltest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/api/release/ -lvreen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/api/debug/ -lvreen
else:symbian: LIBS += -lvreen
else:unix: LIBS += -L$$OUT_PWD/../../../src/api/ -lvreen

INCLUDEPATH += $$PWD/../../../src/api
DEPENDPATH += $$PWD/../../../src/api
