include (../../tests.pri)

TARGET = tst_symbian_framepool
TEMPLATE = app

# Compiles unit test support in framepool sources
DEFINES += FRAME_POOL_UNIT_TEST

QT += declarative svg

INCLUDEPATH = ../../../src/symbian
DEPENDPATH = ../../../src/symbian

SOURCES += tst_framepool.cpp
RESOURCES += sframepool.qrc

HEADERS += sframepool.h
HEADERS += siconpool.h

SOURCES += sframepool.cpp
SOURCES += siconpool.cpp

include (../auto.pri)
