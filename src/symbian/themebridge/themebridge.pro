include (../../../qt-components.pri)

TARGETPATH = com/nokia/symbian/themebridge
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianthemebridgeplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

CONFIG += qt plugin
QT += declarative svg

SOURCES += \
    plugin.cpp \
    sstylewrapper.cpp \
    sstylewrapper_p.cpp \
    sdeclarativeicon.cpp \
    sdeclarativeframe.cpp \
    siconpool.cpp \
    sframepool.cpp \
    sdeclarativeimplicitsizeitem.cpp

HEADERS += \
    sstylewrapper.h \
    sstylewrapper_p.h \
    sdeclarativeicon.h \
    sdeclarativeframe.h \
    siconpool.h \
    sframepool.h \
    sdeclarativeimplicitsizeitem.h

RESOURCES += \
    themebridge.qrc

QML_FILES += \
    qmldir

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0x200346DE
    MMP_RULES += EXPORTUNFROZEN
    MMP_RULES += SMPSAFE

    LIBS += -lfbscli
    LIBS += -lcone
    LIBS += -leikcore

    pluginstub.sources = symbianthemebridgeplugin.dll
    pluginstub.path = /resource/qt/imports/$$TARGETPATH

    resources.path = /resource/qt/imports/$$TARGETPATH
    resources.sources += $$QML_FILES

    DEPLOYMENT = pluginstub resources
}

include(../../../qml.pri)
