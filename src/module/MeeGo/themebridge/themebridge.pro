
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

OTHER_FILES += qmldir

TARGETPATH = com/meego/themebridge

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

otherfiles.files = $$OTHER_FILES
otherfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH


INSTALLS += target otherfiles
