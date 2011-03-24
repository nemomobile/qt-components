include(../../../tests.pri)

TARGET = tst_symbian_declarativeframe
TEMPLATE = app
QT += declarative svg
RESOURCES += sdeclarativeframe.qrc

INCLUDEPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH = $$Q_COMPONENTS_SOURCE_TREE/src/symbian

HEADERS += sframepool.h
HEADERS += sdeclarativeframe.h
HEADERS += siconpool.h

SOURCES += sframepool.cpp
SOURCES += sdeclarativeframe.cpp
SOURCES += siconpool.cpp

SOURCES += tst_declarativeframe.cpp
RESOURCES += symbian.qrc

include(../../auto.pri)
