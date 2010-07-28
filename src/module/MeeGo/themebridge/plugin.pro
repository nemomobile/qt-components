
TEMPLATE = lib
TARGET = meegoplugin
DESTDIR = .
CONFIG += meegotouch qt plugin
QT += declarative

SOURCES += plugin.cpp \
           mstylewrapper.cpp \
           mdeclarativeprimitive.cpp \
           mdeclarativescalableimage.cpp \
           mdeclarativepixmap.cpp \
           mdeclarativeicon.cpp \
           mdeclarativebackground.cpp

HEADERS += mstylewrapper.h \
           mdeclarativeprimitive.h \
           mdeclarativescalableimage.h \
           mdeclarativepixmap.h \
           mdeclarativeicon.h \
           mdeclarativebackground.h

OTHER_FILES += ../qmldir
