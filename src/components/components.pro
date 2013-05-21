include (../../qt-components.pri)
TARGETPATH = Qt/labs/components
TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtcomponentsplugin)
INCLUDEPATH += $$PWD $$PWD/models

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
CONFIG += qt plugin
QT += quick

DEFINES += QT_BUILD_COMPONENTS_LIB

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)

include(models/models.pri)

include(../../qml.pri)
