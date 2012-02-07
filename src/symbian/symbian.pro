include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian.1.1
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin_1_1_2)
INCLUDEPATH += $$PWD $$PWD/indicators

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
CONFIG += qt plugin copy_native install_native
QT += declarative svg
mobility {
    MOBILITY += feedback systeminfo
    QT += network
}

SOURCES += \
    plugin.cpp \
    sdeclarative.cpp \
    sdeclarativefocusscopeitem.cpp \
    sdeclarativeicon.cpp \
    sdeclarativeimageprovider.cpp \
    sdeclarativeimplicitsizeitem.cpp \
    sdeclarativeinputcontext.cpp \
    sdeclarativemagnifier.cpp \
    sdeclarativemaskedimage.cpp \
    sdeclarativescreen.cpp \
    sdeclarativesharedstatusbar.cpp \
    sdeclarativescreen_p.cpp \
    sdeclarativescreen_p_resize.cpp \
    sdeclarativescreen_p_sensor.cpp \
    sdeclarativestyle.cpp \
    sdeclarativestyleinternal.cpp \
    siconpool.cpp \
    smousegrabdisabler.cpp \
    snetworkinfo.cpp \
    spopupmanager.cpp \
    sstyleengine.cpp \
    sstylefactory.cpp \
    indicators/sdeclarativeindicatorcontainer.cpp \
    indicators/sdeclarativenetworkindicator.cpp \
    ssnapshot.cpp

symbian {
    SOURCES += \
        sdeclarativeinputcontext_p_symbian.cpp \
        sdeclarativetouchinput.cpp \
        stimeobserver.cpp
} else {
    SOURCES += \
        sdeclarativeinputcontext_p.cpp
}

symbian:symbian_internal {
    SOURCES += \
        sbatteryinfo_symbian.cpp  \
        sdeclarativesharedstatusbar_p_symbian.cpp \
        ssharedstatusbarsubscriber.cpp \
        indicators/sdeclarativeincallindicator.cpp \
        indicators/sdeclarativeindicator.cpp \
        indicators/sdeclarativeindicatordata.cpp \
        indicators/sdeclarativeindicatordatahandler.cpp \
        indicators/sdeclarativenetworkindicator_p_symbian.cpp \
        indicators/sdeclarativestatuspanedatasubscriber.cpp
} else {
    SOURCES += \
        sbatteryinfo.cpp \
        sdeclarativesharedstatusbar_p.cpp \
        indicators/sdeclarativenetworkindicator_p.cpp
}

HEADERS += \
    sbatteryinfo.h \
    sdeclarative.h \
    sdeclarativefocusscopeitem.h \
    sdeclarativeicon.h \
    sdeclarativeimageprovider.h \
    sdeclarativeimplicitsizeitem.h \
    sdeclarativeinputcontext.h \
    sdeclarativemagnifier.h \
    sdeclarativemaskedimage.h \
    sdeclarativemaskedimage_p.h \
    sdeclarativescreen.h \
    sdeclarativescreen_p.h \
    sdeclarativesharedstatusbar.h \
    sdeclarativescreen_p_resize.h \
    sdeclarativescreen_p_sensor.h \
    sdeclarativestyle.h \
    sdeclarativestyleinternal.h \
    siconpool.h \
    smousegrabdisabler.h \
    snetworkinfo.h \
    spopupmanager.h \
    sstyleengine.h \
    sstylefactory.h \
    indicators/sdeclarativeindicatorcontainer.h \
    indicators/sdeclarativenetworkindicator.h \
    indicators/sdeclarativenetworkindicator_p.h \
    ssnapshot.h

symbian: {
    HEADERS += \
        sdeclarativeinputcontext_p_symbian.h \
        sdeclarativetouchinput.h \
        stimeobserver.h
} else {
    HEADERS += \
        sdeclarativeinputcontext_p.h
}

symbian:symbian_internal {
    HEADERS +=  \
        sdeclarativesharedstatusbar_p_symbian.h \
        ssharedstatusbarsubscriber.h \
        indicators/sdeclarativeincallindicator.h \
        indicators/sdeclarativeindicator.h \
        indicators/sdeclarativeindicatordata.h \
        indicators/sdeclarativeindicatordatahandler.h \
        indicators/sdeclarativestatuspanedatasubscriber.h
} else {
    HEADERS += \
        sdeclarativesharedstatusbar_p.h \
}

RESOURCES += \
    symbian.qrc

QML_FILES = \
    qmldir \
    ApplicationWindow.qml \
    AppManager.js \
    Button.qml \
    ButtonColumn.qml \
    ButtonGroup.js \
    ButtonRow.qml \
    BusyIndicator.qml \
    CheckBox.qml \
    CommonDialog.qml \
    ContextMenu.qml \
    Dialog.qml \
    Fader.qml \
    Label.qml \
    ListHeading.qml \
    ListItem.qml \
    ListItemText.qml \
    Menu.qml \
    MenuContent.qml \
    MenuItem.qml \
    MenuLayout.qml \
    Page.qml \
    PageStack.js \
    PageStack.qml \
    PageStackWindow.qml \
    Popup.qml \
    ProgressBar.qml \
    QueryDialog.qml \
    RadioButton.qml \
    RectUtils.js \
    ScrollBar.qml \
    ScrollDecorator.qml \
    SectionScroller.js \
    SectionScroller.qml \
    SelectionDialog.qml \
    SelectionListItem.qml \
    Slider.qml \
    StatusBar.qml \
    StatusBarDefault.qml \
    StatusBarShared.qml \
    Switch.qml \
    TabBar.qml \
    TabBarLayout.qml \
    TabButton.qml \
    TabGroup.js \
    TabGroup.qml \
    TextArea.qml \
    TextField.qml \
    TextMagnifier.qml \
    TextContextMenu.qml \
    TextSelectionHandle.qml \
    TextTouchController.qml \
    TextTouchTools.qml \
    ToolBar.qml \
    ToolBarLayout.qml \
    ToolButton.qml \
    ToolTip.qml \
    Window.qml

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x2003DE93
    MMP_RULES += SMPSAFE
    VERSION = 10.1.2

    LIBS += -lws32 // For CWsScreenDevice
    LIBS += -lcone // For EikonEnv / CoeEnv
    LIBS += -leikcore // For EikonEnv
    LIBS += -leikcoctl // For CEikStatusPane
    LIBS += -lhal   // For calculating DPI values
    LIBS += -lavkon // For AknAppui

    symbian_internal {
        LIBS += -laknicon // For AknIconUtils
        LIBS += -laknlayout2
        LIBS += -laknnotify // For CAknSmallIndicator
        LIBS += -laknskins // For AknsUtils
        LIBS += -lbafl // For TResourceReader
        LIBS += -lfbscli // For CFbsBitmap
        LIBS += -lcdlengine
        LIBS += -laknlayout2scalable
        LIBS += -lbitgdi
    }

    BLD_INF_RULES.prj_exports += "qtcomponents_1_1.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponents_1_1.iby)"
    BLD_INF_RULES.prj_exports += "qtcomponentsnative.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponentsnative.iby)"

    vendor_info = \
            " " \
            "; Localised Vendor name" \
            "%{\"Nokia\"}" \
            " " \
            "; Unique Vendor name" \
            ":\"Nokia\"" \
            " "

    package.pkg_prerules += vendor_info
    DEPLOYMENT += package

    exists($${EPOCROOT}epoc32/release/winscw/udeb/z/system/install/series60v5.4.sis) {
        DEFINES += S60_V54_OR_HIGHER
    }
}

win32: LIBS += -lpsapi # for allocated memory info

include(../../qml.pri)




