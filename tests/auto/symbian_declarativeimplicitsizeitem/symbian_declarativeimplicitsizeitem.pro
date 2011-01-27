include (../../tests.pri)

TARGET = tst_symbian_declarativeimplicitsizeitem
TEMPLATE = app
QT += declarative svg

INCLUDEPATH += ../../../src/symbian/themebridge
DEPENDPATH += ../../../src/symbian/themebridge

HEADERS += sdeclarativeimplicitsizeitem.h
SOURCES += sdeclarativeimplicitsizeitem.cpp

SOURCES += tst_sdeclarativeimplicitsizeitem.cpp

include (../auto.pri)
