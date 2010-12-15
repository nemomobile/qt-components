include (../../qt-components.pri)

TARGETPATH = Qt/labs/components
TEMPLATE = lib
TARGET = $$qtLibraryTarget(qtcomponentsplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$TARGETPATH
INCLUDEPATH += $$PWD $$PWD/models

CONFIG += qt plugin
QT += declarative

DEFINES += QT_BUILD_COMPONENTS_LIB

QML_FILES += \
    qmldir \
    Checkable.qml \
    CheckableGroup.qml \
    CheckableGroup.js

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0x200346E1
    MMP_RULES += EXPORTUNFROZEN
    MMP_RULES += SMPSAFE

    pluginstub.sources = qtcomponentsplugin.dll
    pluginstub.path = /resource/qt/imports/$$TARGETPATH

    resources.path = /resource/qt/imports/$$TARGETPATH
    resources.sources += $$QML_FILES

    DEPLOYMENT += pluginstub resources
}

HEADERS += qglobalenums.h

SOURCES += plugin.cpp

include(kernel/kernel.pri)

include(models/models.pri)

include(../../qml.pri)
