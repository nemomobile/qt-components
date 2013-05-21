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
import com.nokia.meego 1.1

Page {
    id: playbackPage

    property bool playing: false

    function play()
    {
        playing = true
    }

    function stop()
    {
        playing = false
    }

    RectButton {
        id: changeToolbarButton
        title: "Play"
        width: 320
        anchors.centerIn: parent
        onClicked: if(!playing){ play(); title="Stop" } else { stop(); title= "Play"; }
    }

    tools: Item {
        id: playbackToolbar
        anchors.fill: parent

        ToolIcon {
            iconId: "toolbar-back"; onClicked: pageStack.pop();
            anchors.left: parent.left
            anchors.top: parent.top
        }

        Item {
            anchors.fill: parent

            ToolIcon {
                id: playButton
                anchors.centerIn: parent
                iconId: "toolbar-mediacontrol-play"
                onClicked: playbackPage.play();
            }

            ToolIcon {
                id: stopButton
                anchors.centerIn: parent
                iconId: "toolbar-mediacontrol-stop"
                opacity: 0
                onClicked: playbackPage.stop();
            }

            ToolIcon {
                iconId: "toolbar-mediacontrol-next"
                anchors.left: playButton.right
                anchors.leftMargin: 64
                anchors.top: parent.top
            }

            ToolIcon {
                iconId: "toolbar-mediacontrol-next"
                anchors.right: playButton.left
                anchors.rightMargin: 64
                anchors.top: parent.top
            }
        }

        states: [
            State {
                name: "playState"
                when: playbackPage.playing
                PropertyChanges {
                    target: playButton
                    opacity: 0
                }
                PropertyChanges {
                    target: stopButton
                    opacity: 1
                }
            },
            State {
                name: "stopState"
                when: !playbackPage.playing
                PropertyChanges {
                    target: playButton
                    opacity: 1
                }
                PropertyChanges {
                    target: stopButton
                    opacity: 0
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { property: "opacity"; duration: 400 }
            }
        ]
    }
}
