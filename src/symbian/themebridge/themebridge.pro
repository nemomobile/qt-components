include (../../../qt-components.pri)

TARGETPATH = com/nokia/symbian/themebridge
TEMPLATE = lib
TARGET = $$qtLibraryTarget(symbianthemebridgeplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin install_native
QT += declarative svg

SOURCES += \
    plugin.cpp \
    sstylewrapper.cpp \
    sstylewrapper_p.cpp \
    sdeclarativeicon.cpp \
    sdeclarativeframe.cpp \
    siconpool.cpp \
    sframepool.cpp \
    sdeclarativeimageprovider.cpp \
    sdeclarativeimplicitsizeitem.cpp

HEADERS += \
    sdeclarativeframe.h \
    sdeclarativeicon.h \
    sdeclarativeimageprovider.h \
    sdeclarativeimplicitsizeitem.h \
    sframepool.h \
    siconpool.h \
    sstylewrapper.h \
    sstylewrapper_p.h

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
}

include(../../../qml.pri)
