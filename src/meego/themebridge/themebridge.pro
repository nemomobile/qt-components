TEMPLATE = lib
TARGET = meegothemebridgeplugin
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
CONFIG += meegotouch qt plugin
QT += declarative

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
           mdeclarativebackground.cpp

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
           mdeclarativebackground.h

QML_FILES += qmldir

TARGETPATH = com/meego/themebridge
include(../../../qml.pri)
