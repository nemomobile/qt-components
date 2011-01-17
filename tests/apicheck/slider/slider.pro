include (../../../qt-components.pri)
include (../../tests.pri)
include (../base/apicheckbase.pri)

TARGET = apicheck_slider
TEMPLATE = app
QT += declarative

SOURCES += apicheck_slider.cpp
OTHER_FILES += ApiCheckSlider.qml
