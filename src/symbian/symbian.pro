include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
CONFIG += qt plugin copy_native install_native
QT += declarative svg
mobility {
    MOBILITY += feedback systeminfo
    QT += network
}

SOURCES += \
    plugin.cpp \
    sbatteryinfo.cpp \
    sdeclarative.cpp \
    sdeclarativefocusscopeitem.cpp \
    sdeclarativeicon.cpp \
    sdeclarativeimageprovider.cpp \
    sdeclarativeimplicitsizeitem.cpp \
    sdeclarativemaskedimage.cpp \
    sdeclarativescreen.cpp \
    sdeclarativestyle.cpp \
    sdeclarativestyleinternal.cpp \
    siconpool.cpp \
    snetworkinfo.cpp \
    sstyleengine.cpp \
    sstylefactory.cpp \
    sstylewrapper.cpp \
    sstylewrapper_p.cpp

HEADERS += \
    sbatteryinfo.h \
    sdeclarative.h \
    sdeclarativefocusscopeitem.h \
    sdeclarativeicon.h \
    sdeclarativeimageprovider.h \
    sdeclarativeimplicitsizeitem.h \
    sdeclarativemaskedimage.h \
    sdeclarativemaskedimage_p.h \
    sdeclarativescreen.h \
    sdeclarativescreen_p.h \
    sdeclarativestyle.h \
    sdeclarativestyleinternal.h \
    siconpool.h \
    snetworkinfo.h \
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
    ButtonColumn.qml \
    ButtonGroup.js \
    ButtonRow.qml \
    BusyIndicator.qml \
    CheckBox.qml \
    ChoiceList.qml \
    CommonDialog.qml \
    ContextMenu.qml \
    Dialog.qml \
    Fader.qml \
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
    Popup.qml \
    ProgressBar.qml \
    QueryDialog.qml \
    RadioButton.qml \
    ScrollBar.qml \
    ScrollBarSizer.qml \
    ScrollDecorator.qml \
    SectionScroller.js \
    SectionScroller.qml \
    SectionScrollerLabel.qml \
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
    ToolBar.qml \
    ToolBarLayout.qml \
    ToolButton.qml \
    ToolTip.qml \
    Window.qml

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x200346DD
    MMP_RULES += EXPORTUNFROZEN
    MMP_RULES += SMPSAFE

    LIBS += -lws32
    LIBS += -lcone
    LIBS += -leikcore
    LIBS += -lavkon
    LIBS += -lhal

    BLD_INF_RULES.prj_exports += "qtcomponents.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponents.iby)"

    stubsis = \
        "START EXTENSION app-services.buildstubsis" \
        "OPTION SISNAME symbianplugin_stub" \
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

    header = "$${LITERAL_HASH}{\"symbianplugin\"},(0x200346DD),1,0,0,TYPE=SA,RU"
    package.pkg_prerules += vendor_info header
    DEPLOYMENT += package
}

include(../../qml.pri)
