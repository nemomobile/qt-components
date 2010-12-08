include (../../qt-components.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtcomponentsplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
INCLUDEPATH += $$PWD

CONFIG += qt plugin
QT += declarative

DEFINES += QT_BUILD_COMPONENTS_LIB

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)

include(models/models.pri)
INCLUDEPATH += $$PWD/models

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

TARGETPATH = Qt/labs/components
include(../../qml.pri)
