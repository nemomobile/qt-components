include(../../../tests.pri)

TARGET = tst_symbian_dialog
TEMPLATE = app
QT += declarative

SOURCES += tst_dialog.cpp
OTHER_FILES += tst_dialog.qml

include(../../auto.pri)
