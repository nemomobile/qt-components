
TEMPLATE = lib
TARGET = meegoplugin
DESTDIR = .
CONFIG += meegotouch qt plugin
QT += declarative

SOURCES += plugin.cpp \
           mstylewrapper.cpp \
           mdeclarativescalableimage.cpp

HEADERS += mstylewrapper.h \
           mdeclarativescalableimage.h

OTHER_FILES += ../qmldir
