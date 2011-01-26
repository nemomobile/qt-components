include (../../tests.pri)

TARGET = tst_symbian_declarativeicon
TEMPLATE = app
QT += declarative svg

INCLUDEPATH = ../../../src/symbian/themebridge
DEPENDPATH = ../../../src/symbian/themebridge

HEADERS += sdeclarativeicon.h
SOURCES += tst_declarativeicon.cpp
SOURCES += sdeclarativeicon.cpp
SOURCES += siconpool.cpp
RESOURCES += themebridge.qrc

include (../auto.pri)

