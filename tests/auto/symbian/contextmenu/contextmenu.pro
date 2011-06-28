include(../../../tests.pri)

TARGET = tst_symbian_contextmenu
TEMPLATE = app
QT += declarative

SOURCES += tst_contextmenu.cpp
OTHER_FILES += tst_contextmenu.qml

include(../../auto.pri)
