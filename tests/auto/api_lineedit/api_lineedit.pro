include (../../../qt-components.pri)
include (../../tests.pri)

TARGET = tst_quickcomponentslineedit
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentslineedit.cpp
OTHER_FILES += tst_quickcomponentslineedit.qml

include (../auto.pri)
