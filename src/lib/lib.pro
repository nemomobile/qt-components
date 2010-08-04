TEMPLATE = lib
TARGET = QtComponents

DESTDIR = $$OUT_PWD/../../lib

QT += declarative opengl
CONFIG += build_lib
DEFINES += QDECLARATIVEWINDOW_BUILD_LIB

include(../../qt-components.pri)

PUBLIC_HEADERS += \
    qdeclarativewindow.h
SOURCES += \
    qdeclarativewindow.cpp

HEADERS += $$PUBLIC_HEADERS

INCLUDEPATH = $$PWD

install_headers.files = $$PUBLIC_HEADERS
install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target install_headers

