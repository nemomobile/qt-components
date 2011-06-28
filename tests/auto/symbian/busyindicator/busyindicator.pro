include(../../../tests.pri)

TARGET = tst_symbian_busyindicator
TEMPLATE = app
QT += declarative

SOURCES += tst_busyindicator.cpp
OTHER_FILES += tst_busyindicator.qml

include(../../auto.pri)
