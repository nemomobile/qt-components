include (../../qt-components.pri)

TARGETPATH = com/meego
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegoplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin
QT += declarative
!win32:!macx: QT += dbus

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

QML_FILES = \
        qmldir \
        Action.qml \
        ActionManager.js \
        BasicListItem.qml \
        BusyIndicator.qml \
        Button.qml \
        ButtonColumn.qml \
        ButtonRow.qml \
        ButtonGroup.js \
        CheckBox.qml \
        Dialog.qml \
        DialogTitleBar.qml \
        EscapeButton.qml \
        Fader.qml \
        HomeButton.qml \
        IconButton.qml \
        Label.qml \
        Menu.qml \
        MenuItem.qml \
        ListItem.qml \
        Page.qml \
        PageStack.qml \
        PageStack.js \
        QueryDialog.qml \
        QueryPanel.qml \
        ProgressBar.qml \
        ScrollDecorator.qml \
        ScrollDecoratorSizer.qml \
        TextArea.qml \
        TextField.qml \
        ToolBar.qml \
        ToolBarLayout.qml \
        ToolButton.qml \
        ToolItem.qml \
        Slider.qml \
        SliderTemplate.qml \
        Switch.qml \
        TitleMenuButton.qml \
        UIConstants.js \
        Window.qml \
        WindowDecoration.qml

include(../../qml.pri)
