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

import QtQuick 1.0
import Qt.labs.components.native 1.0

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
