include(../../../tests.pri)

TARGET = tst_symbian_tabbar
TEMPLATE = app
QT += declarative

SOURCES += tst_tabbar.cpp
OTHER_FILES += tst_tabbar.qml

include(../../auto.pri)
