TARGET = scalefm
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle

SOURCES += src/scalefm.cpp
RESOURCES += scalefm.qrc

symbian {
    CONFIG(symbian_internal) {
        TARGET.UID3 = 0x2003DE90
        TARGET.EPOCALLOWDLLDATA = 1
        BLD_INF_RULES.prj_exports += "scalefm.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(scalefm.iby)"
        VERSION = 10.1.2

        stubsis = \
            "START EXTENSION app-services.buildstubsis" \
            "OPTION SISNAME scalefm_stub" \
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
        header = "$${LITERAL_HASH}{\"scalefm\"},(0x2003DE90),1,1,2,TYPE=SA,RU"
        package.pkg_prerules += vendor_info header
        DEPLOYMENT += package
    } else {
        TARGET.UID3 = 0xE0000022
    }
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
}

OTHER_FILES += \
    core/scalefm.qml \
    core/DisplaySelectionDialog.qml \
    core/FrequencyStrip.qml \
    core/MagnifyingTool.qml \
    core/MobileView.qml \
    core/RadioConsole.qml \
    core/RadioControls.qml \
    core/StationListItem.qml \
    core/StationListView.qml \
    core/StationModel.qml \
    core/TabletView.qml \
    core/ViewContainer.qml \
    core/profileloader.js \
    resources/layouts/AppStyle.qml \
    resources/layouts/1024_600/170/AppStyle.qml \
    resources/layouts/1024_768/130/AppStyle.qml \
    resources/layouts/480_320/180/AppStyle.qml \
    resources/layouts/640_360/185/AppStyle.qml \
    resources/layouts/640_360/210/AppStyle.qml \
    resources/layouts/640_480/325/AppStyle.qml \
    resources/layouts/864_480/245/AppStyle.qml \
    resources/layouts/Normal/High/AppStyle.qml
