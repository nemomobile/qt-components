TEMPLATE = app
QT += declarative

CONFIG -= app_bundle

# Input
SOURCES += main.cpp

OTHER_FILES += \
    qml/*.qml \
    qml/*.js

desktop.files = qmlcomponentextrasgallery.desktop
desktop.path = $$INSTALL_PREFIX/share/applications

target.path = $$INSTALL_PREFIX/bin

RESOURCES = qml/qmlfiles.qrc

INSTALLS += target desktop
