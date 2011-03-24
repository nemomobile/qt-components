include(../../../tests.pri)

TARGET = tst_symbian_switch
TEMPLATE = app
QT += declarative

SOURCES += tst_switch.cpp
OTHER_FILES += tst_switch.qml

include(../../auto.pri)
