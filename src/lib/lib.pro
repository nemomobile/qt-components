TEMPLATE = lib
TARGET = QtComponents

DESTDIR = $$OUT_PWD/../../lib

QT += declarative
CONFIG += build_lib

include(../../qt-components.pri)

PUBLIC_HEADERS += \
    qdeclarativewindow.h
HEADERS += \
    qwindowobject_p.h

meego {
    SOURCES += \
        qdeclarativewindow_meego.cpp \
        qwindowobject_meego.cpp
} else {
    SOURCES += \
        qdeclarativewindow_desktop.cpp \
        qwindowobject_desktop.cpp
}

HEADERS += $$PUBLIC_HEADERS

INCLUDEPATH = $$PWD

install_headers.files = $$PUBLIC_HEADERS
install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target install_headers

