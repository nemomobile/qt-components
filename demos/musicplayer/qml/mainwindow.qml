/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

//![0]
import QtQuick 1.0
import Qt.labs.components.native 1.0
import MediaKeysObserver 1.0
import "Storage.js" as Storage

Window {
    id: mainwindow

    property Item musicPickerPage

    Player {
        id: player
        onPlaylistLoaded: {
            if (player.playlistModel.count == 0) {
                pageStack.clear()
                pageStack.push([controlPage, playlistPage, musicPickerPage])
            }
        }
        Component.onCompleted: {
            Storage.initialize()

            Storage.getPlaylist(playlist)
            playlistLoaded()

            var res = Storage.getSetting("volume")
            if (res != "Unknown")
                volume = parseFloat(res)

            res = Storage.getSetting("repeat")
            if (res != "Unknown")
                repeat = res == "true"

            res = Storage.getSetting("shuffle")
            if (res != "Unknown")
                shuffle = res == "true"

            res = Storage.getSetting("index")
            if (res != "Unknown") {
                index = parseInt(res)
                refreshSong()
            }
        }

        Component.onDestruction: {
            Storage.setSetting("volume", volume)
            Storage.setSetting("repeat", repeat)
            Storage.setSetting("shuffle", shuffle)
            Storage.setSetting("index", index)
            Storage.setPlaylist(playlist)
        }

        MediaKeysObserver {
            id: mediakeysobserver

            property int key

            onMediaKeyClicked: {
                switch (key) {
                case MediaKeysObserver.EVolIncKey:
                    audio.volume += 0.1
                    break

                case MediaKeysObserver.EVolDecKey:
                    audio.volume -= 0.1
                    break

                case MediaKeysObserver.EStopKey:
                    stop()
                    break

                case MediaKeysObserver.EBackwardKey:
                    previous()
                    break

                case MediaKeysObserver.EForwardKey:
                    next()
                    break

                case MediaKeysObserver.EPlayPauseKey:
                    if (playing)
                        pause()
                    else
                        play()

                    break
                }
            }

            onMediaKeyPressed: {
                mediakeysobserver.key = key
                timer.start()
            }

            onMediaKeyReleased: {
                timer.stop()
            }
        }

        Timer {
            id: timer
            interval: 300
            repeat: true
            onTriggered: {
                switch (mediakeysobserver.key) {
                case MediaKeysObserver.EVolIncKey:
                    player.volume += 0.1
                    break

                case MediaKeysObserver.EVolDecKey:
                    player.volume -= 0.1
                    break
                }
            }
        }
    }

    StatusBar {id: statusbar}

    PageStack {
        id: pageStack

        anchors.top: statusbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top
        toolBar: toolbar

        ControlsPage {
            id: controlPage

            playlistVisible:  !inPortrait
            backgroundImage: inPortrait ?  "qrc:///qml/images/bg_prt.png" : "qrc:///qml/images/bg_lsc.png"
        }

        PlaylistPage {
            id: playlistPage
            backgroundImage: inPortrait ?  "qrc:///qml/images/bg_prt.png" : "qrc:///qml/images/bg_lsc.png"
        }

    }

    ToolBar {
        id: toolbar

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    //Set the image manually as there is no binding to musicPickerPage created
    //dynamically below
    onInPortraitChanged:{
        if (musicPickerPage)
            musicPickerPage.backgroundImage = inPortrait ?  "qrc:///qml/images/bg_prt.png" : "qrc:///qml/images/bg_lsc.png"
    }

    Component.onCompleted: {
        // Create the musicPickerPage depending on document gallery support
        var musicPicker = documentGallery ? "GalleryPage.qml" : "FilePickerPage.qml"
        var musicPickerComponent = Qt.createComponent(musicPicker);
        musicPickerPage = musicPickerComponent.createObject(pageStack)
        musicPickerPage.backgroundImage = inPortrait ?  "qrc:///qml/images/bg_prt.png" : "qrc:///qml/images/bg_lsc.png"
        pageStack.push(controlPage)
    }
}
//![0]
