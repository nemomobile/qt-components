TEMPLATE = lib
TARGET = QtComponents

DESTDIR = $$OUT_PWD/../../lib

QT += declarative
CONFIG += build_lib

include(../../qt-components.pri)

PUBLIC_HEADERS += qdeclarativewindow.h

meego {
    SOURCES += qdeclarativewindow_meego.cpp
} else {
    SOURCES += qdeclarativewindow_desktop.cpp
}

HEADERS += \
    qwindowobject_p.h deviceorientation_p.h

maemo5 {
    QT += dbus
    SOURCES += deviceorientation_maemo5.cpp
} else {
    SOURCES += deviceorientation.cpp
}

HEADERS += $$PUBLIC_HEADERS

INCLUDEPATH = $$PWD

install_headers.files = $$PUBLIC_HEADERS
install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target install_headers

