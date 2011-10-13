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

import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    property int numOfSelectedItems: 0
    property alias removeMode: playlist.removeMode
    property alias backgroundImage:  background.source

    function clearSelection(remove) {
        var currentSongRemoved = false
        for (var i = 0; i < player.playlistModel.count; i++) {
            if (player.playlistModel.get(i).selected) {
                if (remove) {
                    if (i == player.index)
                        currentSongRemoved = true

                    player.remove(i)
                    i--
                } else {
                    player.playlistModel.setProperty(i, "selected", false)
                }
            }
        }
        if (currentSongRemoved)
            player.refreshSong()

        numOfSelectedItems = 0
    }

    tools: playlistTools

    Image {
        id: background

        anchors.fill: parent
        z: -1
    }

    Component {
        id: heading

        PageHeading {
            width: parent.width
            text: qsTr("Playlist")
        }
    }

    Playlist {
        id: playlist

        anchors.fill: parent
        header: heading
    }

    ToolBarLayout {
        id: playlistTools

        visible: !removeMode

        ToolButton {
            flat: true
            iconSource:"qrc:///qml/images/tb_back.svg"
            onClicked: pageStack.pop()
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_plus.svg"
            onClicked: pageStack.push(musicPickerPage)
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_minus.svg"
            enabled: player.playlistModel.count
            onClicked: {
                removeMode = true
                //![0]
                toolbar.setTools(removeTools, "replace")
                //![0]
            }
        }
    }

    ToolBarLayout {
        id: removeTools

        visible: removeMode

        ToolButton {
            text: qsTr("REMOVE")
            enabled: numOfSelectedItems > 0
            onClicked: {
                clearSelection(true)
                removeMode = false
                toolbar.setTools(playlistTools, "replace")
            }
        }

        ToolButton {
            text: qsTr("CANCEL")
            onClicked: {
                clearSelection(false)
                removeMode = false
                toolbar.setTools(playlistTools, "replace")
            }
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_menu.svg"
            onClicked: viewMenu.open()
        }
    }

    //![1]
    Menu {
        id: viewMenu

        content:
            Column {
                width: viewMenu.width

                MenuItem {
                    text: qsTr("Select All")

                    onClicked: {
                        viewMenu.close()
                        for (var i = 0; i < player.playlistModel.count; i++)
                            player.playlistModel.setProperty(i, "selected", true)

                        numOfSelectedItems = player.playlistModel.count
                    }
                }

                MenuItem {
                    text: qsTr("Unselect All")

                    onClicked: {
                        viewMenu.close()
                        for (var i = 0; i < player.playlistModel.count; i++)
                            player.playlistModel.setProperty(i, "selected", false)

                        numOfSelectedItems = 0
                    }
                }
            }
    }
    //![1]
}
