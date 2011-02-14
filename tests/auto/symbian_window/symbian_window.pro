include (../../tests.pri)

TARGET = tst_window
TEMPLATE = app
QT += declarative

INCLUDEPATH += ../../../src/symbian
SOURCES += tst_window.cpp
OTHER_FILES += tst_window.qml

include (../auto.pri)
