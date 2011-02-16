include (../../../qt-components.pri)

TARGET = qmlgallery
TEMPLATE = app
QT += declarative
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD

SOURCES += gallery.cpp

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x200346E5
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = CAP_APPLICATION
    gallery_qmls.sources = *.qml
    DEPLOYMENT += gallery_qmls
    BLD_INF_RULES.prj_exports += "qmlgallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlgallery.iby)"
}

OTHER_FILES += \
    gallery.qml \
    SampleColumn.qml
