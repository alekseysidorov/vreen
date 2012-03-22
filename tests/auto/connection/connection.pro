#-------------------------------------------------
#
# Project created by QtCreator 2012-03-21T17:38:12
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_connectiontest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_connectiontest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../api/release/ -lvk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../api/debug/ -lvk
else:symbian: LIBS += -lvk
else:unix: LIBS += -L$$OUT_PWD/../../../api/ -lvk

INCLUDEPATH += $$PWD/../../../api
DEPENDPATH += $$PWD/../../../api
