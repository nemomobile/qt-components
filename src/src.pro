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
           qdatetimemodel.h \
           qprogressbarmodel.h \
           style.h \
           teststyle.h \
           qmlstyle.h \
           button.h \
           qgraphicsbutton.h \
           qgraphicsbutton_p.h \
	   qdeclarativebutton.h \
	   qdeclarativebutton_p.h \
	   qdeclarativeslider.h \
	   qdeclarativeslider_p.h

SOURCES += qrangemodel.cpp \
           qbuttonmodel.cpp \
           qlineeditmodel.cpp \
           qdatetimemodel.cpp \
           qprogressbarmodel.cpp \
           style.cpp \
           teststyle.cpp \
           qmlstyle.cpp \
           button.cpp \
           qgraphicsbutton.cpp \
	   qdeclarativebutton.cpp \
	   qdeclarativeslider.cpp
