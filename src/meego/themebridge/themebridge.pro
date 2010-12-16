TEMPLATE = lib
TARGET = meegothemebridgeplugin
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
CONFIG += meegotouch qt plugin
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

TARGETPATH = com/meego/themebridge
include(../../../qml.pri)
