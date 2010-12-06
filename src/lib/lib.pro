TEMPLATE = lib
TARGET = $$qtLibraryTarget(QtComponents)

DESTDIR = $$OUT_PWD/../../lib

QT += declarative opengl
CONFIG += build_lib
DEFINES += Q_COMPONENTS_BUILD_LIB

include(../../qt-components.pri)

HEADERS += \
    qdeclarativewindow.h
SOURCES += \
    qdeclarativewindow.cpp

macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
    CONFIG += lib_bundle debug_and_release
    CONFIG(debug, debug|release) {
        !build_pass:CONFIG += build_all
    } else { #release
        !debug_and_release|build_pass {
            FRAMEWORK_HEADERS.version = Versions
            FRAMEWORK_HEADERS.files = $$HEADERS
            FRAMEWORK_HEADERS.path = Headers
        }
        QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
    }
} else {
    install_headers.files = $$HEADERS
    install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents
    INSTALLS += install_headers
}

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target
