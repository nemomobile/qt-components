TARGET = tst_remotethemedaemonclient
include(../test_common.pri)

# required for remote graphicssystem
QT += network

!contains(CONFIG, desktop) {
    QT += meegographicssystemhelper
}

SOURCES += \
    tst_remotethemedaemonclient.cpp \
    testthemedaemonserver.cpp \
    testthemedaemonserverthread.cpp \
    ../../components/src/themedaemon/mabstractthemedaemonclient.cpp \
    ../../components/src/themedaemon/mremotethemedaemonclient.cpp \
    ../../components/src/themedaemon/mthemedaemonprotocol.cpp

HEADERS += \
    testthemedaemonserver.h \
    testthemedaemonserverthread.h \
    ../../components/src/themedaemon/mabstractthemedaemonclient.h \
    ../../components/src/themedaemon/mremotethemedaemonclient.h \
    ../../components/src/themedaemon/mthemedaemonprotocol.h
