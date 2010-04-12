TEMPLATE = lib
TARGET = components
DESTDIR = $$OUT_PWD/../lib
DEFINES += QT_BUILD_COMPONENTS_LIB QT_NO_BUTTONGROUP
QT += declarative

include(models/models.pri)
