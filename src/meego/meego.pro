
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegoplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
CONFIG += qt plugin meego
QT += declarative
!macx: QT += dbus

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
        MultiLineEdit.qml \
        ListItem.qml \
        Page.qml \
        pagemanager.js \
        QueryDialog.qml \
        QueryDialog.js \
        QueryPanel.qml \
        PageContainer.qml \
        ProgressBar.qml \
        ScrollDecorator.qml \
        ScrollDecoratorSizer.qml \
        Slider.qml \
        BusyIndicator.qml \
        Switch.qml \
        TitleMenuButton.qml \
        Window.qml \
        WindowDecoration.qml

TARGETPATH = com/meego
include(../../qml.pri)
