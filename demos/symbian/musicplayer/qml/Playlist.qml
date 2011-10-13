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
