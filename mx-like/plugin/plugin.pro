TEMPLATE = lib
TARGET = ../mx/mxlikeplugin
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt plugin
QT += declarative

# Input
HEADERS += mxwindow.h
SOURCES += mxwindow.cpp plugin.cpp
