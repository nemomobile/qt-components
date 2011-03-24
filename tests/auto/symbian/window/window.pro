include(../../../tests.pri)

TARGET = tst_window
TEMPLATE = app
QT += declarative

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian

SOURCES += tst_window.cpp
OTHER_FILES += tst_window.qml

include(../../auto.pri)
