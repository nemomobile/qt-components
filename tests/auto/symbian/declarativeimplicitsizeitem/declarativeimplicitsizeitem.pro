include(../../../tests.pri)

TARGET = tst_symbian_declarativeimplicitsizeitem
TEMPLATE = app
QT += declarative svg

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian

HEADERS += sdeclarativeimplicitsizeitem.h
SOURCES += sdeclarativeimplicitsizeitem.cpp

SOURCES += tst_sdeclarativeimplicitsizeitem.cpp

include(../../auto.pri)
