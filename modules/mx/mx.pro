include (../../qt-components.pri)

TEMPLATE = lib
TARGET = mxlikeplugin
DESTDIR = Mx
CONFIG += qt plugin
QT += declarative

HEADERS += \
        mxlikewindow.h \
        mxlikebuttongroup.h \
	mxtoplevelitem.h \
	mxtoplevelitem_p.h

SOURCES += \
        mxlikewindow.cpp \
        mxlikebuttongroup.cpp \
	mxtoplevelitem.cpp \
        plugin.cpp

