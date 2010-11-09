include (../../qt-components.pri)

TARGETPATH = Qt/labs/components
TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtcomponentsplugin)
DESTDIR = $$PWD # we put the library there so that the autotests can link

CONFIG += qt plugin
QT += declarative

DEFINES += QT_BUILD_COMPONENTS_LIB

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)

include(models/models.pri)
INCLUDEPATH += $$PWD/models

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlimages.files = $$QML_IMAGES
qmlimages.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/images

INSTALLS += target qmlfiles qmlimages
