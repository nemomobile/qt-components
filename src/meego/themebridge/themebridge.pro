include (../../../qt-components.pri)

TARGETPATH = com/meego/themebridge
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegothemebridgeplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin
QT += declarative network
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

SOURCES += plugin.cpp \
           mstylewrapper.cpp \
           mthemebridge.cpp \
           mdeclarativeprimitive.cpp \
           mdeclarativescalableimage.cpp \
           mdeclarativepixmap.cpp \
           mdeclarativepiepixmap.cpp \
           mdeclarativeicon.cpp \
           mdeclarativeiconglow.cpp \
           mdeclarativeimageprovider.cpp \
           mdeclarativemaskedimage.cpp \
           mdeclarativeimplicitsizeitem.cpp \
           mdeclarativebackground.cpp \
           themedaemon/mabstractthemedaemonclient.cpp \
           themedaemon/mlocalthemedaemonclient.cpp \
           themedaemon/mremotethemedaemonclient.cpp \
           themedaemon/mthemedaemonprotocol.cpp

HEADERS += mstylewrapper.h \
           mthemebridge.h \
           mdeclarativeprimitive.h \
           mdeclarativescalableimage.h \
           mdeclarativepixmap.h \
           mdeclarativepiepixmap.h \
           mdeclarativeicon.h \
           mdeclarativeiconglow.h \
           mdeclarativeimageprovider.h \
           mdeclarativemaskedimage.h \
           mdeclarativeimplicitsizeitem.h \
           mdeclarativebackground.h \
           themedaemon/mabstractthemedaemonclient.h \
           themedaemon/mlocalthemedaemonclient.h \
           themedaemon/mremotethemedaemonclient.h \
           themedaemon/mthemedaemonprotocol.h

QML_FILES += qmldir

include(../../../qml.pri)
