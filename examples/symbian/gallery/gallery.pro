TARGET = qmlgallery
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD

ICON = icon.svg
SOURCES += gallery.cpp
RESOURCES += gallery.qrc

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x200346E5
    TARGET.EPOCHEAPSIZE = 0x1000 0x1800000 // 24MB
    TARGET.CAPABILITY = ReadDeviceData SwEvent WriteDeviceData
    gallery_qmls.sources = *.qml
    DEPLOYMENT += gallery_qmls
    BLD_INF_RULES.prj_exports += "gallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlgallery.iby)"
}

OTHER_FILES += \
    gallery.qml \
    SampleColumn.qml
