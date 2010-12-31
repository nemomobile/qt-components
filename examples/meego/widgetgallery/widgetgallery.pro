TEMPLATE = app
QT += declarative

CONFIG -= app_bundle

# Input
SOURCES += widgetgallery.cpp

OTHER_FILES += \
    widgetgallery.qml \
    WidgetGallerySections.qml \
    gallery/*.qml \
    images/*

