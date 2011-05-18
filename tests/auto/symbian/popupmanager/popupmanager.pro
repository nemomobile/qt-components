include(../../../tests.pri)

TARGET = tst_popupmanager
TEMPLATE = app
QT += declarative

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian

SOURCES += tst_popupmanager.cpp
OTHER_FILES += tst_popupmanager.qml

include(../../auto.pri)
