TEMPLATE = app
TARGET = musicplayer
QT += declarative

SOURCES += main.cpp
HEADERS += mediakeysobserver.h

OTHER_FILES += \
    qml/ControlsPage.qml \
    qml/FilePickerPage.qml \
    qml/GalleryPage.qml \
    qml/mainwindow.qml \
    qml/PageHeading.qml \
    qml/Player.qml \
    qml/Playlist.qml \
    qml/PlaylistPage.qml \
    qml/Storage.js

symbian {
    TARGET.UID3 = 0x2003DE92
    SOURCES += mediakeysobserver.cpp
    LIBS += -lremconinterfacebase -lremconcoreapi
    DEFINES += DOCUMENT_GALLERY
    DEFINES += FULLSCREEN
    TARGET.EPOCHEAPSIZE = 0x20000 0x2000000
    ICON = icon.svg

    stubsis = \
        "START EXTENSION app-services.buildstubsis" \
        "OPTION SISNAME musicplayer_stub" \
        "OPTION SRCDIR ."\
        "END"
    BLD_INF_RULES.prj_extensions = stubsis

    vendor_info = \
            " " \
            "; Localised Vendor name" \
            "%{\"Nokia\"}" \
            " " \
            "; Unique Vendor name" \
            ":\"Nokia, Qt\"" \
            " "

    header = "$${LITERAL_HASH}{\"musicplayer\"},(0x2003DE92),1,0,0,TYPE=SA,RU"
    package.pkg_prerules += vendor_info header
    DEPLOYMENT += package
}

RESOURCES += resources.qrc
