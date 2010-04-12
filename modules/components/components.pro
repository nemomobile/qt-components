include (../../qt-components.pri)

TEMPLATE = lib
TARGET = componentsqmlplugin
DESTDIR = ../../plugin/Components
CONFIG += qt plugin
QT += declarative

SOURCES += plugin.cpp
