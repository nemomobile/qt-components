TEMPLATE = app
equals(QT_MAJOR_VERSION, 4): QT += declarative
equals(QT_MAJOR_VERSION, 5) {
    include(../../src/compat/compat.pri)
    QT += quick widgets
}
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
