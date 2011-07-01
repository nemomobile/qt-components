include(../../../tests.pri)

TARGET = tst_declarativeinputcontext
TEMPLATE = app
QT += declarative

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian
DEPENDPATH += $$Q_COMPONENTS_SOURCE_TREE/src/symbian

SOURCES += tst_declarativeinputcontext.cpp
OTHER_FILES += tst_declarativeinputcontext.qml

include(../../auto.pri)
