include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_quickcomponentsswitch
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsswitch.cpp
OTHER_FILES += tst_quickcomponentsswitch.qml

include (../auto.pri)
