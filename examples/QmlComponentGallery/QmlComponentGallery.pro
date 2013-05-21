TEMPLATE = app
TARGET = QmlComponentGallery5
target.path = $$INSTALL_PREFIX/bin

include(../../src/compat/compat.pri)
QT += quick
TARGET = QmlComponentGallery5

CONFIG -= app_bundle

# Input
SOURCES += main.cpp

OTHER_FILES += \
    qml/*.qml

desktop.files = qmlcomponentgallery.desktop
desktop.path = $$INSTALL_PREFIX/share/applications

RESOURCES = qml/qmlfiles.qrc

INSTALLS += target desktop
