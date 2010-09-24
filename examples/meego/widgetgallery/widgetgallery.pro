include(../../../qt-components.pri)
TEMPLATE = app
QT += declarative
CONFIG += meego contextsubscriber

# Input
SOURCES += widgetgallery.cpp

OTHER_FILES += \
    widgetgallery.qml \
    ListItem.qml \
    BasicListItem.qml

