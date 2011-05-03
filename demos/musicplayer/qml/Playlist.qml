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

Item {
    property bool removeMode: false
    property alias header: listview.header

    Component {
        id: playlistDelegate

        Item {
            height: 75
            width: listview.width

            Rectangle {
                height: parent.height
                width: 4
                color: "#2d2875"
                visible: index % 2
            }

            Row {
                anchors { top: parent.top; bottom: parent.bottom; margins: 10 }
                width: parent.width

                Item {
                    id: numText

                    width: 50
                    height: parent.height
                    anchors.top: parent.top

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: index != player.index
                        font.pixelSize: 22
                        font.letterSpacing: -1
                        color: "white"
                        text: index + 1 + "."
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Image {
                        anchors.top: parent.top
                        anchors.topMargin: 4
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: index == player.index
                        source: player.playing ? "qrc:///qml/images/i_current_track_play.svg" : "qrc:///qml/images/i_current_track_pause.svg"
                    }
                }

                Column {
                    width: (listview.width > 300) ? (parent.width - timeText.width - numText.width) : (parent.width - timeText.width)
                    spacing: 3

                    Text {
                        id: titleText

                        width: parent.width
                        elide: Text.ElideRight
                        font.pixelSize: 22
                        font.letterSpacing: -1
                        color: "white"
                        text: title
                    }

                    Text {
                        id: artistText

                        width: parent.width
                        elide: Text.ElideRight
                        text: artist
                        font.pixelSize: 18
                        color: "white"
                    }
                }

                Item {
                    width: 60
                    height: parent.height

                    Text {
                        id: timeText

                        width: parent.width
                        visible: !removeMode && listview.width > 300
                        anchors.top: parent.top
                        horizontalAlignment: Text.AlignHCenter
                        elide: Text.ElideRight
                        font.pixelSize: 22
                        font.letterSpacing: -1
                        color: "white"
                        text: time
                    }

                    //![0]
                    CheckBox {
                        visible: removeMode
                        anchors.centerIn: parent
                        checked: selected
                    }
                    //![0]
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (removeMode) {
                        var selected = player.playlistModel.get(index).selected
                        player.playlistModel.setProperty(index, "selected", !selected)
                        if (selected)
                            numOfSelectedItems--
                        else
                            numOfSelectedItems++

                    } else {
                        if (player.index == index) {
                            if (player.playing)
                                player.pause()
                            else
                                player.play()

                        } else {
                            player.index = index
                            player.refreshSong()
                        }
                    }
                }
            }
        }
    }

    //![1]
    ListView {
        id: listview

        anchors.fill: parent
        model: player.playlistModel
        delegate: playlistDelegate
        currentIndex: player.index
        cacheBuffer: height
        clip: true
        highlightMoveDuration: 500

        ScrollDecorator {
            flickableItem: parent
        }
    }
    //![1]
}
