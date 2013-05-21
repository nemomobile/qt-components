TEMPLATE = app
TARGET = musicplayer
QT += declarative
CONFIG += mobility
SOURCES += main.cpp

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

RESOURCES += resources.qrc
