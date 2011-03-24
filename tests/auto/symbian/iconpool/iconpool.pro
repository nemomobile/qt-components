include(../../../tests.pri)

TARGET = tst_symbian_iconpool
TEMPLATE = app

# Compiles unit test support in iconpool sources
DEFINES += ICON_POOL_UNIT_TEST

QT += declarative svg

INCLUDEPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian

SOURCES += tst_iconpool.cpp
RESOURCES += siconpool.qrc

HEADERS += siconpool.h
SOURCES += siconpool.cpp

include(../../auto.pri)
