include (../../../tests.pri)

INCLUDEPATH += ../../../../src/meego

QT += network
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

TEMPLATE = app
SOURCES += \
    tst_mremotethemedaemonclient.cpp \
    testthemedaemonserver.cpp \
    testthemedaemonserverthread.cpp \
    ../../../../src/meego/themedaemon/mabstractthemedaemonclient.cpp \
    ../../../../src/meego/themedaemon/mremotethemedaemonclient.cpp \
    ../../../../src/meego/themedaemon/mthemedaemonprotocol.cpp

HEADERS += \
    testthemedaemonserver.h \
    testthemedaemonserverthread.h \
    ../../../../src/meego/themedaemon/mabstractthemedaemonclient.h \
    ../../../../src/meego/themedaemon/mremotethemedaemonclient.h \
    ../../../../src/meego/themedaemon/mthemedaemonprotocol.h

include (../../auto.pri)
