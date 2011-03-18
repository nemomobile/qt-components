include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
CONFIG += qt plugin copy_native install_native
QT += declarative svg
mobility:MOBILITY += feedback

SOURCES += \
    plugin.cpp \
    sdeclarative.cpp \
    sdeclarativeframe.cpp \
    sdeclarativeicon.cpp \
    sdeclarativeimageprovider.cpp \
    sdeclarativeimplicitsizeitem.cpp \
    sdeclarativemaskedimage.cpp \
    sdeclarativescreen.cpp \
    sdeclarativestyle.cpp \
    sdeclarativestyleinternal.cpp \
    sdeclarativewindowdecoration.cpp \
    sframepool.cpp \
    siconpool.cpp \
    sstyleengine.cpp \
    sstylefactory.cpp \
    sstylewrapper.cpp \
    sstylewrapper_p.cpp

HEADERS += \
    sdeclarative.h \
    sdeclarativeframe.h \
    sdeclarativeicon.h \
    sdeclarativeimageprovider.h \
    sdeclarativeimplicitsizeitem.h \
    sdeclarativemaskedimage.h \
    sdeclarativemaskedimage_p.h \
    sdeclarativescreen.h \
    sdeclarativescreen_p.h \
    sdeclarativestyle.h \
    sdeclarativestyleinternal.h \
    sdeclarativewindowdecoration.h \
    sframepool.h \
    siconpool.h \
    sstyleengine.h \
    sstylefactory.h \
    sstylewrapper.h \
    sstylewrapper_p.h

RESOURCES += \
    symbian.qrc

QML_FILES = \
    qmldir \
    ApplicationWindow.qml \
    AppManager.js \
    Button.qml \
    BusyIndicator.qml \
    CheckBox.qml \
    ChoiceList.qml \
    ContextMenu.qml \
    Dialog.qml \
    Fader.qml \
    ListHeading.qml \
    ListItem.qml \
    ListItemText.qml \
    Menu.qml \
    MenuBaseOld.qml \
    MenuContent.qml \
    MenuItem.qml \
    MenuLayout.qml \
    ObjectMenu.qml \
    Page.qml \
    PageStack.js \
    PageStack.qml \
    Popup.qml \
    ProgressBar.qml \
    RadioButton.qml \
    ScrollBar.qml \
    ScrollDecorator.qml \
    SectionScroller.js \
    SectionScroller.qml \
    SectionScrollerLabel.qml \
    SelectionListItem.qml \
    Slider.qml \
    TabBar.qml \
    TabBarLayout.qml \
    TabButton.qml \
    TabGroup.js \
    TabGroup.qml \
    TextArea.qml \
    TextField.qml \
    ToolBar.qml \
    ToolBarLayout.qml \
    ToolButton.qml \
    ToolTip.qml \
    ViewMenu.qml \
    Window.qml

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x200346DD
    MMP_RULES += EXPORTUNFROZEN
    MMP_RULES += SMPSAFE

    LIBS += -lfbscli
    LIBS += -lws32
    LIBS += -lcone
    LIBS += -lapgrfx
    LIBS += -leikcoctl
    LIBS += -leikcore
    LIBS += -lavkon
    LIBS += -lhal

    BLD_INF_RULES.prj_exports += "qtcomponents.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponents.iby)"
}

include(../../qml.pri)
