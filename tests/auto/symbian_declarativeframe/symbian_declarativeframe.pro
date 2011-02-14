include (../../tests.pri)

TARGET = tst_symbian_declarativeframe
TEMPLATE = app
QT += declarative svg
RESOURCES += sdeclarativeframe.qrc

INCLUDEPATH = ../../../src/symbian/themebridge
DEPENDPATH = ../../../src/symbian/themebridge

HEADERS += sframepool.h
HEADERS += sdeclarativeframe.h
HEADERS += siconpool.h

SOURCES += sframepool.cpp
SOURCES += sdeclarativeframe.cpp
SOURCES += siconpool.cpp

SOURCES += tst_declarativeframe.cpp
RESOURCES += themebridge.qrc

include (../auto.pri)
