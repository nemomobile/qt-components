include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_quickcomponentsmultilineedit
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsmultilineedit.cpp
OTHER_FILES += tst_quickcomponentsmultilineedit.qml

include (../auto.pri)
