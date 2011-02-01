include (../../../qt-components.pri)
QT += declarative core gui

TARGET = qmlgallery
CONFIG -= app_bundle

TEMPLATE = app

win32 {
    DESTDIR = $$PWD
}

SOURCES += gallery.cpp

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x200346E5
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = CAP_APPLICATION
    gallery_qmls.sources = *.qml
    DEPLOYMENT += gallery_qmls
    BLD_INF_RULES.prj_exports += "qmlgallery.iby /epoc32/rom/include/core/app/qmlgallery.iby"
}

OTHER_FILES += \
    gallery.qml \
    SampleColumn.qml
