#-------------------------------------------------
#
# Project created by QtCreator 2012-03-29T15:23:03
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_rostertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_rostertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/api/release/ -lvreen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/api/debug/ -lvreen
else:symbian: LIBS += -lvreen
else:unix: LIBS += -L$$OUT_PWD/../../../src/api/ -lvreen

INCLUDEPATH += $$PWD/../../../src/api \
    $$PWD/../../

DEPENDPATH += $$PWD/../../../src/api
