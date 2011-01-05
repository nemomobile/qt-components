include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_quickcomponentstoolbar
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentstoolbar.cpp
OTHER_FILES += tst_quickcomponentstoolbar.qml 

include (../auto.pri)
