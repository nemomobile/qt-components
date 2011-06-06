include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentsmenu
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsmenu.cpp
OTHER_FILES += tst_quickcomponentsmenu.qml

include (../../auto.pri)
