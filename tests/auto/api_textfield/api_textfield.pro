include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_api_textfield
TEMPLATE = app
QT += declarative

SOURCES += tst_api_textfield.cpp
OTHER_FILES += tst_api_textfield.qml

include (../auto.pri)
