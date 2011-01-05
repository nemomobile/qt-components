include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_api_pagestack
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentspagestack.cpp
OTHER_FILES += tst_quickcomponentspagestack.qml

include (../auto.pri)
