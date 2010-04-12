include (../../qt-components.pri)

TEMPLATE = lib
TARGET = mxlikeplugin
DESTDIR = Mx
CONFIG += qt plugin
QT += declarative

HEADERS += \
        mxlikewindow.h \
        mxlikebuttongroup.h 

SOURCES += \
        mxlikewindow.cpp \
        mxlikebuttongroup.cpp \
        plugin.cpp

