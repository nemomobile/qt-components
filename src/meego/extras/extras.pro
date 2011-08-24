include (../../../qt-components.pri)

TARGETPATH = com/nokia/extras com/meego/extras
TEMPLATE = lib
TARGET = $$qtLibraryTarget(meegoextrasplugin)
INCLUDEPATH += $$PWD

win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release
CONFIG += qt plugin
QT += declarative network

meegotouch {
    DEFINES += HAVE_MEEGOTOUCH
}

HEADERS += mdatetimehelper.h
SOURCES += plugin.cpp mdatetimehelper.cpp

QML_FILES += \
    qmldir \
    constants.js\
    CountBubble.qml \
    DatePickerDialog.qml \
    InfoBanner.qml \
    ListDelegate.qml \
    ListButton.qml \
    MoreIndicator.qml \
    NetPromoterScore.qml \
    PageIndicator.qml \
    RatingIndicator.qml \
    SystemBanner.qml \
    TimePickerDialog.qml \
    Tumbler.qml \
    TumblerNew.qml \
    TumblerDialogNew.qml \
    Tumbler.js \
    TumblerColumn.qml \
    TumblerIndexHelper.js \
    TumblerTemplate.qml \
    TumblerButton.qml \
    TumblerDialog.qml \
    style/ListButtonStyle.qml \
    style/NegativeButtonStyle.qml \
    style/PositiveButtonStyle.qml \
    style/TumblerButtonStyle.qml


##### to be removed after April 8th #####

#IMPORTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/com/meego/extras
#IMPORT_DESTDIR = $$replace(IMPORTDIR, /, $$QMAKE_DIR_SEP)

#contains(QMAKE_HOST.os, Windows):isEmpty(QMAKE_SH) {
#    CHK_DIR_EXISTS_MKDIR = $$QMAKE_CHK_DIR_EXISTS $$IMPORT_DESTDIR $$QMAKE_MKDIR $$IMPORT_DESTDIR
#} else {
#    CHK_DIR_EXISTS_MKDIR = $$QMAKE_CHK_DIR_EXISTS $$IMPORT_DESTDIR || $$QMAKE_MKDIR $$IMPORT_DESTDIR
#}

#mkdir.commands += $$CHK_DIR_EXISTS_MKDIR
#QMAKE_EXTRA_TARGETS += mkdir

#ARGUMENTS = $$DESTDIR/$(TARGET) $$IMPORT_DESTDIR
#QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)

#for(qmlfile, QML_FILES) {
#    ARGUMENTS = $$_PRO_FILE_PWD_/$$qmlfile $$IMPORT_DESTDIR
#    target = copy_native_$$lower($$basename(qmlfile))
#    target = $$replace(target, \\., _)
#    commands = $${target}.commands
#    $$commands += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
#    depends = $${target}.depends
#    $$depends += mkdir
#    QMAKE_EXTRA_TARGETS += $$target
#    POST_TARGETDEPS += $$target
#}

#import_target.CONFIG += no_check_exist executable
#import_target.files = $$DESTDIR/$(TARGET)
#import_target.path = $$[QT_INSTALL_IMPORTS]/com/meego/extras
#import_qmlfiles.files = $$QML_FILES
#import_qmlfiles.path = $$[QT_INSTALL_IMPORTS]/com/meego/extras
#INSTALLS += import_target import_qmlfiles

#########################################


include(../../../qml.pri)
