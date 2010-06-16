include (../qt-components.pri)

TEMPLATE = lib
TARGET = qtcomponentsplugin
DESTDIR = QtComponents
CONFIG += qt plugin
QT += declarative

SOURCES += plugin.cpp

OTHER_FILES += \
    QtComponents/qmldir
