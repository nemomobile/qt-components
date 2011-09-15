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
    id: changeToolbarPage

    RectButton {
        id: changeToolbarButton
        title: "Change toolbar"
        checkable: true
        width: 320
        anchors.centerIn: parent
    }

    tools: Item {
        id: toolbar

        anchors.fill: parent
        clip: true

        ToolIcon {
            iconId: "toolbar-back"
            anchors.left: parent.left
            anchors.top: parent.top

            onClicked: pageStack.pop()
        }

        Item {
            id: messagingPanel
            width: 320
            x: 255
            height:72

            ToolIcon {
                id: newChat
                anchors.centerIn: parent
                iconId: "toolbar-new-chat"
            }

            ToolIcon {
                id: newMessageButton
                anchors.right: newChat.left
                anchors.rightMargin: 64
                anchors.top:  parent.top
                iconId: "toolbar-new-message"
            }

            ToolIcon {
                id: shareButton
                iconId: "toolbar-share"
                anchors.left: newChat.right
                anchors.leftMargin: 64
                anchors.top: parent.top
            }
        }

        Item {
            id: searchPanel

            width: 320
            x: 255
            height: 72

            opacity: 0
            y: 50

            ToolIcon {
                id: searchButton
                anchors.centerIn: parent
                iconId: "toolbar-search"
            }

            ToolIcon {
                id: toolsButton
                anchors.right: searchButton.left
                anchors.rightMargin: 64
                anchors.top:  parent.top
                iconId: "toolbar-tools"
            }

            ToolIcon {
                id: sendEmailButton
                iconId: "toolbar-send-email"
                anchors.left: searchButton.right
                anchors.leftMargin: 64
                anchors.top: parent.top
            }
        }

        ToolIcon {
            iconId: "toolbar-view-menu"
            anchors.right: parent.right
            anchors.top: parent.top
        }

        states: [
            State {
                name: "searchPanelState"
                when: changeToolbarButton.toogled
                PropertyChanges {
                    target: messagingPanel
                    opacity: 0
                    y: -50
                }

                PropertyChanges {
                    target: searchPanel
                    opacity: 1
                    y: 0
                }
            },
            State {
                name: "messagingPanelState"
                when: !changeToolbarButton.toogled
                PropertyChanges {
                    target: messagingPanel
                    opacity: 1
                    y: 0
                }

                PropertyChanges {
                    target: searchPanel
                    opacity: 0
                    y: -50
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { property: "opacity"; duration: 200 }
                NumberAnimation { property: "y"; duration: 200 }
            }
        ]
    }
}
