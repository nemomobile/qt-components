TEMPLATE = app
QT += declarative
CONFIG += meego contextsubscriber
include(../../../qt-components.pri)

# Input
SOURCES += widgetgallery.cpp

OTHER_FILES += \
    widgetgallery.qml \
    MeeGoDelegate.qml

