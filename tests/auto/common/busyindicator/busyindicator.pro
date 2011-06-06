include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentsbusyindicator
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsbusyindicator.cpp
OTHER_FILES += tst_quickcomponentsbusyindicator.qml

include (../../auto.pri)
