
TEMPLATE = lib
TARGET = meegoplugin
CONFIG += qt plugin meego
QT += declarative dbus

include (../../qt-components.pri)

contextsubscriber {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    LIBS += -lcontextsubscriber
}

SOURCES += \
    plugin.cpp \
    mdeclarativestatusbar.cpp \
    mdeclarativescreen.cpp \
    msnapshot.cpp

HEADERS += \
    mdeclarativestatusbar.h \
    mdeclarativescreen.h \
    msnapshot.h

INCLUDEPATH += $$PWD

QML_FILES = \
        qmldir \
        Button.qml \
        ButtonColumn.qml \
        ButtonRow.qml \
        ButtonGroup.js \
        EscapeButton.qml \
        IconButton.qml \
        Label.qml \
        LineEdit.qml \
        CheckBox.qml \
        Page.qml \
        QueryDialog.qml \
        QueryPanel.qml \
        Slider.qml \
        Spinner.qml \
        Switch.qml \
        TitleBar.qml \
        Window.qml \
        pagemanager.js

OTHER_FILES += $$QML_FILES

TARGETPATH = com/meego

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
