include (../../qt-components.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianplugin)
CONFIG += qt plugin
QT += declarative

SOURCES += \
    plugin.cpp \
    sdeclarativescreen.cpp \
    sdeclarativewindowdecoration.cpp

HEADERS += \
    sdeclarativescreen.h \
    sdeclarativewindowdecoration.h

INCLUDEPATH += $$PWD

QML_FILES = \
    qmldir

OTHER_FILES += $$QML_FILES

TARGETPATH = com/nokia/symbian

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

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

INSTALLS += target qmlfiles
