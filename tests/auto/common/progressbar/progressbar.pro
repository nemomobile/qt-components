include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentsprogressbar
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsprogressbar.cpp
OTHER_FILES += tst_quickcomponentsprogressbar.qml

include (../../auto.pri)
