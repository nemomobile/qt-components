TEMPLATE = lib
TARGET = QtComponents

DESTDIR = $$OUT_PWD/../lib
DEFINES += QT_BUILD_COMPONENTS_LIB QT_NO_BUTTONGROUP

QT += declarative
CONFIG += build_lib

include(../qt-components.pri)

include(models/models.pri)
include(styles/styles.pri)
include(kernel/kernel.pri)
include(chrome/chrome.pri)

HEADERS += $$PUBLIC_HEADERS

INCLUDEPATH = $$PWD
DEFINES += QT_BUILD_COMPONENTS_LIB

install_headers.files = $$PUBLIC_HEADERS
install_headers.path = $$[QT_INSTALL_HEADERS]/QtComponents

target.path = $$[QT_INSTALL_LIBS]

INSTALLS += target install_headers
