include(../../../../tests.pri)

TARGET = tst_symbian_tumbler
TEMPLATE = app
QT += declarative

SOURCES += tst_tumbler.cpp
OTHER_FILES += tst_tumbler.qml

include(../../../auto.pri)
