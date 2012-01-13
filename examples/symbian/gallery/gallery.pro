TARGET = qmlgallery
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle
include (../shared/utils.pri)

ICON = icon.svg
SOURCES += gallery.cpp
RESOURCES += gallery.qrc

symbian {
    CONFIG(symbian_internal) {
        TARGET.UID3 = 0x200346E5
        TARGET.EPOCALLOWDLLDATA = 1
        BLD_INF_RULES.prj_exports += "gallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlgallery.iby)"
        VERSION = 10.1.2

        stubsis = \
            "START EXTENSION app-services.buildstubsis" \
            "OPTION SISNAME qmlgallery_stub" \
            "OPTION SRCDIR ."\
            "END"
        BLD_INF_RULES.prj_extensions = stubsis

        vendor_info = \
            " " \
            "; Localised Vendor name" \
            "%{\"Nokia\"}" \
            " " \
            "; Unique Vendor name" \
            ":\"Nokia\"" \
            " "
        header = "$${LITERAL_HASH}{\"qmlgallery\"},(0x200346E5),1,1,2,TYPE=SA,RU"
        package.pkg_prerules += vendor_info header
        DEPLOYMENT += package
    } else {
        TARGET.UID3 = 0xE0000021
    }
    TARGET.EPOCHEAPSIZE = 0x1000 0x1800000 # 24MB
}

OTHER_FILES += \
    gallery.qml \
    galleryContent.qml \
    SampleColumn.qml
