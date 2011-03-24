include(../../../tests.pri)

TARGET = tst_declarativescreen
TEMPLATE = app
QT += declarative

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian

SOURCES += tst_declarativescreen.cpp
OTHER_FILES += tst_declarativescreen.qml
OTHER_FILES += tst_declarativescreen2.qml

include(../../auto.pri)
