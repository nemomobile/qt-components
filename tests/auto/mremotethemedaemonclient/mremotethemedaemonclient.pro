include (../../tests.pri)

INCLUDEPATH += ../../../src/meego/themebridge

QT += network
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}
CONFIG += testcase

TEMPLATE = app
SOURCES += \
    tst_mremotethemedaemonclient.cpp \
    testthemedaemonserver.cpp \
    testthemedaemonserverthread.cpp \
    ../../../src/meego/themebridge/themedaemon/mabstractthemedaemonclient.cpp \
    ../../../src/meego/themebridge/themedaemon/mremotethemedaemonclient.cpp \
    ../../../src/meego/themebridge/themedaemon/mthemedaemonprotocol.cpp

HEADERS += \
    testthemedaemonserver.h \
    testthemedaemonserverthread.h \
    ../../../src/meego/themebridge/themedaemon/mabstractthemedaemonclient.h \
    ../../../src/meego/themebridge/themedaemon/mremotethemedaemonclient.h \
    ../../../src/meego/themebridge/themedaemon/mthemedaemonprotocol.h

#include (../auto.pri)
