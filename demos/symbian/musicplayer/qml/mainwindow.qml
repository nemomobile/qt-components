/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

//![0]
import QtQuick 1.1
import com.nokia.symbian 1.1
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
