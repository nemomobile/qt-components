
TEMPLATE = lib
TARGET = meegothemebridgeplugin
DESTDIR = .
CONFIG += meegotouch qt plugin
QT += declarative

SOURCES += plugin.cpp \
           mstylewrapper.cpp \
           mdeclarativeprimitive.cpp \
           mdeclarativescalableimage.cpp \
           mdeclarativepixmap.cpp \
           mdeclarativepiepixmap.cpp \
           mdeclarativeicon.cpp \
           mdeclarativeiconglow.cpp \
           mdeclarativemaskedimage.cpp \
           mdeclarativeimplicitsizeitem.cpp \
           mdeclarativebackground.cpp

HEADERS += mstylewrapper.h \
           mdeclarativeprimitive.h \
           mdeclarativescalableimage.h \
           mdeclarativepixmap.h \
           mdeclarativepiepixmap.h \
           mdeclarativeicon.h \
           mdeclarativeiconglow.h \
           mdeclarativemaskedimage.h \
           mdeclarativeimplicitsizeitem.h \
           mdeclarativebackground.h

OTHER_FILES += qmldir

TARGETPATH = com/meego/themebridge

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

otherfiles.files = $$OTHER_FILES
otherfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH


INSTALLS += target otherfiles
