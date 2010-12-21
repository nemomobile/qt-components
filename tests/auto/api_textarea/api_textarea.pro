include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_api_textarea
TEMPLATE = app
QT += declarative

SOURCES += tst_api_textarea.cpp
OTHER_FILES += tst_api_textarea.qml

include (../auto.pri)
