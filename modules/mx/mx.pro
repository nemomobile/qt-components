include (../../qt-components.pri)

TEMPLATE = lib
TARGET = mxlikeplugin
DESTDIR = Mx
CONFIG += qt plugin
QT += declarative

HEADERS += \
        mxlikewindow.h \
        mxlikebuttongroup.h \
        mxliketextinput.h

SOURCES += \
        mxlikewindow.cpp \
        mxlikebuttongroup.cpp \
        mxliketextinput.cpp \
        plugin.cpp

