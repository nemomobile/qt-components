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
