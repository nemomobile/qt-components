TEMPLATE = app
QT += declarative
target.path = $$INSTALL_PREFIX/bin

CONFIG -= app_bundle

# Input
SOURCES += main.cpp

OTHER_FILES += \
    qml/*.qml

desktop.files = tutorialapplication.desktop
desktop.path = $$INSTALL_PREFIX/share/applications

RESOURCES = qml/qmlfiles.qrc

INSTALLS += target desktop
