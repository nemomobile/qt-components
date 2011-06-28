include(../../../tests.pri)

TARGET = tst_symbian_menu
TEMPLATE = app
QT += declarative

SOURCES += tst_menu.cpp
OTHER_FILES += tst_menu.qml

include(../../auto.pri)
