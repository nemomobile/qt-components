TARGET = qmlextrasgallery
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle
include (../shared/utils.pri)

SOURCES += extrasgallery.cpp

symbian {
    CONFIG(symbian_internal) {
        TARGET.UID3 = 0x2003A914
        TARGET.EPOCALLOWDLLDATA = 1
        BLD_INF_RULES.prj_exports += "qmlextrasgallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlextrasgallery.iby)"
        VERSION = 10.1.2

        stubsis = \
            "START EXTENSION app-services.buildstubsis" \
            "OPTION SISNAME qmlextrasgallery_stub" \
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
        header = "$${LITERAL_HASH}{\"qmlextrasgallery\"},(0x2003A914),1,1,2,TYPE=SA,RU"
        package.pkg_prerules += vendor_info header
        DEPLOYMENT += package
    } else {
        TARGET.UID3 = 0xE0000023
    }
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
}

OTHER_FILES += \
    main.qml \
    DatePickerDialog.qml \
    InfoBannerPage.qml \
    RatingIndicator.qml \
    TimePickerDialog.qml \
    Tumbler.qml

RESOURCES += \
    extrasgallery.qrc
