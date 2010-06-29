include (../qt-components.pri)

# hack for now to get something
CONFIG += mx

TEMPLATE = lib
TARGET = qtcomponentsplugin
CONFIG += qt plugin
QT += declarative

SOURCES += plugin.cpp

mx:include(Mx/Mx.pri)

TARGETPATH = Qt/labs/components

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlimages.files = $$QML_IMAGES
qmlimages.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/images

INSTALLS += target qmlfiles qmlimages
