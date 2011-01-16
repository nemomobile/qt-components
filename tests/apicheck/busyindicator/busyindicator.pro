include (../../../qt-components.pri)
include (../../tests.pri)
include (../base/apicheckbase.pri)

TARGET = apicheck_busyindicator
TEMPLATE = app
QT += declarative

SOURCES += apicheck_busyindicator.cpp
OTHER_FILES += ApiCheckBusyIndicator.qml
