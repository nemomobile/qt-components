TEMPLATE = app
QT += declarative
target.path = $$INSTALL_PREFIX/bin

include(../util/util.pri)

CONFIG -= app_bundle

# Input
SOURCES += main.cpp

OTHER_FILES += \
    qml/*.qml

desktop.files = qmlcomponentgallery.desktop
desktop.path = $$INSTALL_PREFIX/share/applications

RESOURCES = qml/qmlfiles.qrc

INSTALLS += target desktop
