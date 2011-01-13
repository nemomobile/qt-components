include (../../qt-components.pri)

TARGETPATH = com/nokia/symbian
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin
QT += declarative

SOURCES += \
    plugin.cpp \
    sdeclarativeprogressbaranimation.cpp \
    sdeclarativescreen.cpp \
    sdeclarativewindowdecoration.cpp

HEADERS += \
    sdeclarative.h \
    sdeclarativeprogressbaranimation.h \
    sdeclarativescreen.h \
    sdeclarativewindowdecoration.h

QML_FILES = \
    qmldir \
    Button.qml \
    CheckBox.qml \
    ProgressBar.qml \
    TextField.qml

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
    LIBS += -lcdlengine
    LIBS += -leikcoctl
    LIBS += -leikcore
    LIBS += -lavkon

    BLD_INF_RULES.prj_exports += "qtcomponents.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtcomponents.iby)"

    pluginstub.sources = symbianplugin.dll
    pluginstub.path = /resource/qt/imports/$$TARGETPATH

    resources.path = /resource/qt/imports/$$TARGETPATH
    resources.sources = $$QML_FILES

    DEPLOYMENT = pluginstub resources
}

include(../../qml.pri)
