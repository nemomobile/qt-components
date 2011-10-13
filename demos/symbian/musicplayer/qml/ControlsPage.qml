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

//![0]
Page {
    tools: controlsTools
    //![0]
    clip: true
    property bool playlistVisible
    property alias backgroundImage:  background.source

    Playlist {
        anchors { left: parent.left; right: controlContainer.left }
        height: parent.height
        visible: controlsTools.playlistVisible
    }

    Image {
        id: background
        anchors.fill: parent
        z: -1
    }

    Item {
        id: controlContainer

        height: parent.height
        width: Math.min(parent.width, parent.height)
        anchors.right: parent.right

        Row {
            id: controlButtons

            height: playpauseButton.height
            anchors {
                left: parent.left
                leftMargin: 10
                right: parent.right
                rightMargin: 10
                bottom: parent.bottom
                bottomMargin: 22
            }

            Button {
                id: rewindButton

                width: parent.width / 3
                iconSource: "qrc:///qml/images/i_rewind.svg"
                enabled: player.count > 0

                onClicked: {
                    if (player.position > 2000)
                        player.position = 0
                    else
                        player.previous()
                }
            }

            Button {
                id: playpauseButton

                width: parent.width / 3
                iconSource: player.playing ? "qrc:qml/images/i_pause.svg" : "qrc:qml/images/i_play.svg"
                enabled: player.count > 0

                onClicked: player.playing ? player.pause() : player.play()
            }

            Button {
                id: nextButton

                width: parent.width / 3
                iconSource: "qrc:///qml/images/i_fast_forward.svg"
                enabled: player.count > 0 && (player.index < player.count - 1 || player.repeat || player.shuffle)

                onClicked: player.next()
            }
        }

        Item {
            id: seekSlider

            anchors { bottom: controlButtons.top; bottomMargin: 25 }
            width: parent.width
            height: slider.height + positionTime.height

            //![1]
            Slider {
                id: slider

                anchors.top: parent.top
                width: parent.width
                maximumValue: player.duration
                stepSize: 1000

                onPressedChanged: {
                    if (!pressed)
                        player.position = value
                }

                Binding {
                    target: slider
                    property: "value"
                    value: player.position
                    when: !slider.pressed
                }
            }
            //![1]

            Text {
                id: positionTime

                anchors { left: parent.left; top: slider.bottom; leftMargin: 15; topMargin: 4 }
                color: "white"
                font.pixelSize: 18
                text: player.positionTime
            }

            Text {
                id: durationTime

                anchors { right: parent.right; top: slider.bottom; rightMargin: 15; topMargin: 4 }
                color: "white"
                font.pixelSize: 18
                text: player.durationTime
            }
        }

        Item {
            id: infoText

            anchors { bottom: seekSlider.top; bottomMargin: 36 }
            width: parent.width
            height: title.height + artist.height + 15
            clip: true

            Text {
                id: artist

                width: parent.width
                anchors { bottom: title.top; bottomMargin: 18 }
                horizontalAlignment: Text.AlignHCenter
                text: player.artist
                color: "white"
                font.pixelSize: 28
                wrapMode: Text.WordWrap
                visible: player.artist != "" ? true : false

                Behavior on text {
                    NumberAnimation {
                        target: artist
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: 1000
                    }
                }
            }

            Text {
                id: title

                width: parent.width
                anchors.bottom: parent.bottom
                horizontalAlignment: Text.AlignHCenter
                text: player.title
                color: "white"
                font.pixelSize: 20
                wrapMode: Text.WordWrap

                Behavior on text {
                    NumberAnimation {
                        target: title
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: 1000
                    }
                }
            }
        }
    }

    //![2]
    ToolBarLayout {
        id: controlsTools

        ToolButton {
            flat: true
            iconSource:"qrc:///qml/images/tb_back.svg"
            onClicked: Qt.quit()
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_random.svg"
            checkable: player.count > 1
            checked: player.shuffle
            onCheckedChanged: player.shuffle = checked
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_repeat.svg"
            checkable:true
            checked: player.repeat
            onCheckedChanged: player.repeat = checked
        }

        ToolButton {
            flat: true
            iconSource: "qrc:///qml/images/tb_list.svg"
            onClicked: pageStack.push(playlistPage)
        }
    }
    //![2]
}
