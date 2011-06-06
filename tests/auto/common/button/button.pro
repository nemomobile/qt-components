include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentsbutton
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsbutton.cpp
OTHER_FILES += tst_quickcomponentsbutton.qml

include (../../auto.pri)
