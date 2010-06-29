TEMPLATE = lib
TARGET = qtcomponents
DESTDIR = $$OUT_PWD/../lib
DEFINES += QT_BUILD_COMPONENTS_LIB QT_NO_BUTTONGROUP
QT += declarative


include(models/models.pri)
include(styles/styles.pri)
include(kernel/kernel.pri)
include(chrome/chrome.pri)

INCLUDEPATH = $$PWD
DEFINES += QT_BUILD_COMPONENTS_LIB
