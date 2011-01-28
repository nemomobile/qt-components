include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin copy_native install_native
QT += declarative

SOURCES += \
    plugin.cpp \
    sdeclarative.cpp \
    sdeclarativescreen.cpp \
    sdeclarativewindowdecoration.cpp

HEADERS += \
    sdeclarative.h \
    sdeclarativescreen.h \
    sdeclarativescreen_p.h \
    sdeclarativewindowdecoration.h

QML_FILES = \
    qmldir \
    AppManager.js \
    Button.qml \
    CheckBox.qml \
    ChoiceList.qml \
    Dialog.qml \
    Fader.qml \
    ListHeading.qml \
    ListItem.qml \
    ListItemText.qml \
    Menu.qml \
    ObjectMenu.qml \
    Page.qml \
    PageStack.js \
    PageStack.qml \
    ProgressBar.qml \
    RadioButton.qml \
    ScrollBar.qml \
    ScrollDecorator.qml \
    Slider.qml \
    TextArea.qml \
    TextField.qml \
    ViewMenu.qml \
    Window.qml

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
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
