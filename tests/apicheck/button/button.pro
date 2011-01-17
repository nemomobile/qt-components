include (../../../qt-components.pri)
include (../../tests.pri)
include (../base/apicheckbase.pri)

TARGET = apicheck_button
TEMPLATE = app
QT += declarative

SOURCES += apicheck_button.cpp
OTHER_FILES += ApiCheckButton.qml
