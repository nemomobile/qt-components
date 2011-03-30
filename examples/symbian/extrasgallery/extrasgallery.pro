TARGET = qmlextrasgallery
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD

SOURCES += extrasgallery.cpp

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x2003A914
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = ReadDeviceData SwEvent WriteDeviceData
    gallery_qmls.sources = *.qml
    DEPLOYMENT += gallery_qmls
    BLD_INF_RULES.prj_exports += "qmlextrasgallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlextrasgallery.iby)"
}

OTHER_FILES += \
    main.qml \
    RatingIndicator.qml \
    Tumbler.qml
