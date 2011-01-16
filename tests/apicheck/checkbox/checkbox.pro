include (../../../qt-components.pri)
include (../../tests.pri)
include (../base/apicheckbase.pri)

TARGET = apicheck_checkbox
TEMPLATE = app
QT += declarative

SOURCES += apicheck_checkbox.cpp
OTHER_FILES += ApiCheckCheckBox.qml
