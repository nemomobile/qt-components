include (../../../qt-components.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianthemebridgeplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
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

OTHER_FILES += \
    qmldir

TARGETPATH = com/nokia/symbian/themebridge

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

otherfiles.files = $$OTHER_FILES
otherfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

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
    resources.sources += $$OTHER_FILES

    DEPLOYMENT = pluginstub resources
}

INSTALLS += target otherfiles
