include (../../qt-components.pri)

TEMPLATE = lib
TARGET = qtcomponentsplugin
CONFIG += qt plugin
QT += declarative

DEFINES += QT_NO_BUTTONGROUP QT_BUILD_COMPONENTS_LIB

SOURCES += plugin.cpp

include(kernel/kernel.pri)

include(models/models.pri)
INCLUDEPATH += $$PWD/models

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

TARGETPATH = Qt/labs/components

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlimages.files = $$QML_IMAGES
qmlimages.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/images

INSTALLS += target qmlfiles qmlimages
