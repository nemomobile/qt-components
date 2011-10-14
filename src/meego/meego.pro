include (../../qt-components.pri)

TARGETPATH = com/nokia/meego com/meego
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegoplugin)
INCLUDEPATH += $$PWD

DEFINES += THEME_DIR=\\\"\"$$THEME_DIR\"\\\"
force-local-theme: DEFINES+=FORCE_LOCAL_THEME

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
CONFIG += qt plugin copy_native install_native
QT += declarative network opengl
!win32:!macx: QT += dbus

!win32:!embedded:!mac:!symbian {
    CONFIG += link_pkgconfig
}

meegotouch {
    DEFINES += HAVE_MEEGOTOUCH
}

maliit {
    DEFINES += HAVE_MALIIT
    PKGCONFIG += maliit-1.0
}
                                
contains(DEFINES, HAVE_MEEGOGRAPHICSSYSTEM) {
    QT += meegographicssystemhelper
}

contains(DEFINES, HAVE_THEMEDAEMON_PROTOCOL_V1) {
    DEFINES += HAVE_THEMEDAEMON_PROTOCOL_V1
}

themedaemonprotocolv1 {
    DEFINES += HAVE_THEMEDAEMON_PROTOCOL_V1
}

contextsubscriber {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    LIBS += -lcontextsubscriber
    SOURCES += mservicelistener.cpp
    HEADERS += mservicelistener.h
    # PKGCONFIG += contextsubscriber
}

meegographicssystem {
    DEFINES += HAVE_MEEGOGRAPHICSSYSTEM
    QT += meegographicssystemhelper
}

xdamage {
    DEFINES += HAVE_XDAMAGE
    PKGCONFIG += xdamage
}

xrandr:!mac {
    DEFINES += HAVE_XRANDR
    LIBS += -lXrandr
}

load(mobilityconfig, true)
contains(MOBILITY_CONFIG, systeminfo) {
    CONFIG += mobility
    MOBILITY += systeminfo
    DEFINES += HAVE_SYSTEMINFO
}

SOURCES += \
    plugin.cpp \
    mdeclarativestatusbar.cpp \
    mdeclarativescreen.cpp \
    msnapshot.cpp \
    minversemousearea.cpp \
    mdeclarativeinputcontext.cpp \
    mdeclarativeimageprovider.cpp \
    mdeclarativeimplicitsizeitem.cpp \
    mdeclarativemaskeditem.cpp \
    mscrolldecoratorsizer.cpp \
    mtexttranslator.cpp \
    mthemeplugin.cpp \
    mwindowstate.cpp \
    mx11wrapper.cpp \
    themedaemon/mabstractthemedaemonclient.cpp \
    themedaemon/mlocalthemedaemonclient.cpp \
    themedaemon/mremotethemedaemonclient.cpp \
    themedaemon/mthemedaemonprotocol.cpp \
    mdeclarativemousefilter.cpp \
    mdeclarativeimattributeextension.cpp \
    mdeclarativeimobserver.cpp \
    mdeclarativeview.cpp \
    shadereffectitem/shadereffect.cpp \
    shadereffectitem/shadereffectitem.cpp \
    shadereffectitem/shadereffectsource.cpp \
    shadereffectitem/shadereffectbuffer.cpp \
    shadereffectitem/scenegraph/qsggeometry.cpp

HEADERS += \
    mdeclarativestatusbar.h \
    mdeclarativescreen.h \
    mdialogstatus.h \
    msnapshot.h \
    mpagestatus.h \
    minversemousearea.h \
    mdeclarativeinputcontext.h \
    mdeclarativeimageprovider.h \
    mdeclarativeimplicitsizeitem.h \
    mdeclarativemaskeditem.h \
    mscrolldecoratorsizer.h \
    mtexttranslator.h \
    mthemeplugin.h \
    mwindowstate.h \
    themedaemon/mabstractthemedaemonclient.h \
    themedaemon/mlocalthemedaemonclient.h \
    themedaemon/mremotethemedaemonclient.h \
    themedaemon/mthemedaemonprotocol.h \
    mdeclarativemousefilter.h \
    mdeclarativeimattributeextension.h \
    mdeclarativeimobserver.h \
    mdeclarativeview.h \
    i18n/mlocalewrapper.h \
    mpageorientation.h \
    mtoolbarvisibility.h \
    mdeclarativeview.h \
    shadereffectitem/glfunctions.h \
    shadereffectitem/shadereffect.h \
    shadereffectitem/shadereffectitem.h \
    shadereffectitem/shadereffectsource.h \
    shadereffectitem/shadereffectbuffer.h \
    shadereffectitem/scenegraph/qsggeometry.h
                            

QML_FILES = \
        qmldir \
        ApplicationWindow.qml \
        style/ApplicationWindowStyle.qml \
        BusyIndicator.qml \
        style/BusyIndicatorStyle.qml \
        Button.qml \
        style/ButtonStyle.qml \
        style/GroupButtonStyle.qml \
        ButtonColumn.qml \
        ButtonRow.qml \
        ButtonGroup.js \
        BasicRow.qml \
        BasicRow.js \
        CheckBox.qml \
        CommonDialog.qml \
        style/CheckBoxStyle.qml \
        ContextMenu.qml \
        style/ContextMenuStyle.qml \
        Dialog.qml \
        Utils.js \
        EditBubble.js \
        EditBubble.qml \
        EditBubbleButton.qml \
        style/EditBubbleStyle.qml \
        style/EditBubbleButtonStyle.qml \
        style/DialogStyle.qml \
        style/QueryDialogStyle.qml \
        style/SelectionDialogStyle.qml \
        style/MenuStyle.qml \
        Fader.qml \
        Label.qml \
        style/LabelStyle.qml \
        AbstractMenu.qml \
        Menu.qml \
        MenuItem.qml \
        MenuLayout.qml \
        style/MenuItemStyle.qml \
        MouseAreaDebug.qml \
        NotificationBanner.qml \
        Page.qml \
        PageStack.qml \
        PageStack.js \
        PageStackWindow.qml \
	style/PageStackWindowStyle.qml \
        Popup.qml \
        QueryDialog.qml \
        SelectionDialog.qml \
        MultiSelectionDialog.qml \
        ProgressBar.qml \
        style/ProgressBarStyle.qml \
        RadioButton.qml \
        style/RadioButtonStyle.qml \
        ScrollDecorator.qml \
        style/ScrollDecoratorStyle.qml \
        ScrollDecoratorSizer.qml \
        SectionScroller.qml \
        style/SectionScrollerStyle.qml \
        SectionScroller.js \
        SectionScrollerLabel.qml \
        SelectionHandles.qml \
        SelectionHandles.js \
        style/SelectionHandlesStyle.qml \
        StatusBar.qml \
        TextArea.qml \
        style/TextAreaStyle.qml \
        TextField.qml \
        style/TextFieldStyle.qml \
        TabButton.qml \
        TabBarLayout.qml \
        style/TabButtonStyle.qml \
        style/Style.qml \
        TabGroup.qml \
        ToolBar.qml \
        ToolBarLayout.js \
        ToolBarLayout.qml \
        style/ToolBarStyle.qml \
        ToolButton.qml \
        style/ToolButtonStyle.qml \
        ToolItem.qml \
        ToolIcon.qml \
        ToolButtonRow.qml \
        style/ToolItemStyle.qml \
        Sheet.qml \
        style/SheetStyle.qml \
        SheetButton.qml \
        style/SheetButtonStyle.qml \
        style/SheetButtonAccentStyle.qml \
        Slider.qml \
        style/SliderStyle.qml \
        SliderTemplate.qml \
        Switch.qml \
        style/SwitchStyle.qml \
        UIConstants.js \
        Window.qml \
        SipSimulator.qml \
        SoftwareInputPanel.qml \
        TextAreaHelper.js \ 
        MultiSelectionDialog.js \
        Magnifier.qml \
        Magnifier.js

include(../../qml.pri)

OTHER_FILES +=
