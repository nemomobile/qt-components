TEMPLATE = app
TARGET = flickr
CONFIG += qt-components
CONFIG -= app_bundle
QT += network declarative

SOURCES += main.cpp
RESOURCES += flickr.qrc

symbian {
    CONFIG(symbian_internal) {
        TARGET.UID3 = 0x2003DE91
        BLD_INF_RULES.prj_exports += "qmlflickr.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlflickr.iby)"
        stubsis = \
            "START EXTENSION app-services.buildstubsis" \
            "OPTION SISNAME flickr_stub" \
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

        header = "$${LITERAL_HASH}{\"flickr\"},(0x2003DE91),1,0,0,TYPE=SA,RU"
        package.pkg_prerules += vendor_info header
        DEPLOYMENT += package
    } else {
        TARGET.UID3 = 0xE0000024
    }
    TARGET.EPOCHEAPSIZE = 0x400000 0x6000000 #64MB
    TARGET.CAPABILITY += NetworkServices
    ICON = flickr.svg
}

OTHER_FILES += \
    qml/*.qml \
    qml/*.js \
    qml/images/* \
    *.svg
