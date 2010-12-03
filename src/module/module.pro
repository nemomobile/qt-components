include (../../qt-components.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtcomponentsplugin)

CONFIG += qt plugin
QT += declarative

DEFINES += QT_BUILD_COMPONENTS_LIB

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)
include(models/models.pri)

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

TARGETPATH = Qt/labs/components
include(../../qml.pri)
