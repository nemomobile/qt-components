TEMPLATE = lib
TARGET = components
DESTDIR = $$OUT_PWD/../lib
DEFINES += QT_BUILD_COMPONENTS_LIB QT_NO_BUTTONGROUP QT_NO_SHORTCUT
QT += declarative

HEADERS += qrangemodel.h \
           qrangemodel_p.h \
           qbuttonmodel.h \
           qbuttonmodel_p.h \
           qlineeditmodel.h \
           style.h \
           teststyle.h \
           qmlstyle.h \
           button.h

SOURCES += qrangemodel.cpp \
           qbuttonmodel.cpp \
           qlineeditmodel.cpp \
           style.cpp \
           teststyle.cpp \
           qmlstyle.cpp \
           button.cpp

