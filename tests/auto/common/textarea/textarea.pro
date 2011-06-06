include (../../../../qt-components.pri)
include (../../../tests.pri)

TARGET = tst_quickcomponentstextarea
TEMPLATE = app
QT += declarative

SOURCES += tst_quickcomponentstextarea.cpp
OTHER_FILES += tst_quickcomponentstextarea.qml

include (../../auto.pri)
