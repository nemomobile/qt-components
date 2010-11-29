
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegoplugin)
CONFIG += qt plugin meego
QT += declarative dbus

include (../../qt-components.pri)

contextsubscriber {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    LIBS += -lcontextsubscriber
}

!win32:!embedded:!mac:!symbian {
    CONFIG += link_pkgconfig
    PKGCONFIG += xdamage
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
        Action.qml \
        ActionButton.qml \
        ActionManager.js \
        BasicListItem.qml \
        Button.qml \
        ButtonColumn.qml \
        ButtonRow.qml \
        ButtonGroup.js \
        CheckBox.qml \
        EscapeButton.qml \
        HomeButton.qml \
        IconButton.qml \
        Label.qml \
        LineEdit.qml \
        ListItem.qml \
        Page.qml \
        pagemanager.js \
        QueryDialog.qml \
        QueryDialog.js \
        QueryPanel.qml \
        PageContainer.qml \
        PositionIndicator.qml \
        ProgressBar.qml \
        Slider.qml \
        BusyIndicator.qml \
        Switch.qml \
        TitleMenuButton.qml \
        Window.qml \
        WindowDecoration.qml

OTHER_FILES += $$QML_FILES

TARGETPATH = com/meego

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
