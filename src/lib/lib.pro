TEMPLATE = lib
TARGET = $$qtLibraryTarget(QtComponents)

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

macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    CONFIG += lib_bundle debug_and_release
    CONFIG(debug, debug|release) {
        !build_pass:CONFIG += build_all
    } else { #release
        !debug_and_release|build_pass {
            FRAMEWORK_HEADERS.version = Versions
            FRAMEWORK_HEADERS.files = $$PUBLIC_HEADERS
            FRAMEWORK_HEADERS.path = Headers
        }
        QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
    }
} else {
    install_headers.files = $$PUBLIC_HEADERS
    install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents
    INSTALLS += install_headers
}

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target
