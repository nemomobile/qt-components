include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian.1.1
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin_1_1)
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
        sbatteryinfo_symbian.cpp \
        indicators/sdeclarativeincallindicator.cpp \
        indicators/sdeclarativeindicator.cpp \
        indicators/sdeclarativeindicatordata.cpp \
        indicators/sdeclarativeindicatordatahandler.cpp \
        indicators/sdeclarativenetworkindicator_p_symbian.cpp \
        indicators/sdeclarativestatuspanedatasubscriber.cpp
} else {
    SOURCES += \
        sbatteryinfo.cpp \
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
    indicators/sdeclarativenetworkindicator_p.h

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
        indicators/sdeclarativeincallindicator.h \
        indicators/sdeclarativeindicator.h \
        indicators/sdeclarativeindicatordata.h \
        indicators/sdeclarativeindicatordatahandler.h \
        indicators/sdeclarativestatuspanedatasubscriber.h
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
    VERSION = 10.1.0

    LIBS += -lws32 // For CWsScreenDevice
    LIBS += -lcone // For EikonEnv / CoeEnv
    LIBS += -leikcore // For EikonEnv
    LIBS += -leikcoctl // For CEikStatusPane
    LIBS += -lavkon // For AknAppui SetOrientationL
    LIBS += -lhal   // For calculating DPI values

    symbian_internal {
        LIBS += -laknicon // For AknIconUtils
        LIBS += -laknlayout2
        LIBS += -laknnotify // For CAknSmallIndicator
        LIBS += -laknskins // For AknsUtils
        LIBS += -lbafl // For TResourceReader
        LIBS += -lfbscli // For CFbsBitmap
        LIBS += -lcdlengine
        LIBS += -laknlayout2scalable
    }

    BLD_INF_RULES.prj_exports += "qtcomponents_1_1.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponents_1_1.iby)"
    BLD_INF_RULES.prj_exports += "qtcomponentsnative.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponentsnative.iby)"

    stubsis = \
        "START EXTENSION app-services.buildstubsis" \
        "OPTION SISNAME symbianplugin_1_1_stub" \
        "OPTION SRCDIR ."\
        "END"
    BLD_INF_RULES.prj_extensions = stubsis

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
}

win32: LIBS += -lpsapi # for allocated memory info

include(../../qml.pri)
