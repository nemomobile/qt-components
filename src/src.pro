TEMPLATE = lib
TARGET = components
DESTDIR = $$OUT_PWD/../lib
DEFINES += QT_BUILD_COMPONENTS_LIB QT_NO_BUTTONGROUP
QT += declarative

include(models/models.pri)

HEADERS += style.h \
           teststyle.h \
           qmlstyle.h \
           qgraphicsbutton.h \
           qgraphicsbutton_p.h \
	   qdeclarativebutton.h \
	   qdeclarativebutton_p.h \
	   qdeclarativeslider.h \
	   qdeclarativeslider_p.h

SOURCES += style.cpp \
           teststyle.cpp \
           qmlstyle.cpp \
           qgraphicsbutton.cpp \
	   qdeclarativebutton.cpp \
	   qdeclarativeslider.cpp
