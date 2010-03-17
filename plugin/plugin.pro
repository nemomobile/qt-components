include (../qt-components.pri)

TEMPLATE = lib
TARGET = componentsqmlplugin
DESTDIR = Components
CONFIG += qt plugin
QT += declarative

SOURCES += plugin.cpp
