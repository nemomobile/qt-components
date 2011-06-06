include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentsdialog
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentsdialog.cpp
OTHER_FILES += tst_quickcomponentsdialog.qml

include (../../auto.pri)
