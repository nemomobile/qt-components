include(../../../tests.pri)

TARGET = tst_symbian_declarativeicon
TEMPLATE = app
QT += declarative svg

INCLUDEPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian

HEADERS += sdeclarativeicon.h
SOURCES += tst_declarativeicon.cpp
SOURCES += sdeclarativeicon.cpp
SOURCES += siconpool.cpp
RESOURCES += symbian.qrc

include(../../auto.pri)

