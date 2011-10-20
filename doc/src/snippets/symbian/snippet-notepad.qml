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

Window {
    id: window

//! [1]
    StatusBar {
        id: statusBar

        Behavior on opacity { PropertyAnimation { duration: 200 } }

        states: [
            State {
                name: "Visible"; when: !inputContext.visible
                PropertyChanges { target: statusBar; y: 0; opacity: 1}
            },

            State {
                name: "Hidden"; when: inputContext.visible
                PropertyChanges { target: statusBar; y: -height; opacity: 0 }
            }
        ]

        transitions: [
            Transition {
                from: "Hidden"; to: "Visible"
                NumberAnimation { target: statusBar; properties: "y"; duration: 200; easing.type: Easing.OutQuad }
            },

            Transition {
                from: "Visible"; to: "Hidden"
                NumberAnimation { target: statusBar; properties: "y"; duration: 200; easing.type: Easing.Linear }
            }
        ]
    }

//! [1]

    ToolBar {
        id: topToolBar

        anchors { left: parent.left; right: parent.right; top: statusBar.bottom }

        tools: ToolBarLayout {
            ToolButton { text: "Untitled"; enabled: false; flat: true }
            ToolButton { iconSource: "toolbar-add" }
        }
    }

    Item {
        id: filler

        anchors { left: parent.left; right: parent.right; top: topToolBar.bottom }
        height: platformStyle.graphicSizeMedium * 2 - topToolBar.height

        Text {
            id: dateText

            anchors {
                left: parent.left; leftMargin: platformStyle.paddingSmall
                verticalCenter: parent.verticalCenter
            }

            color: platformStyle.colorNormalLight
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium; weight: Font.Bold }
            text: Qt.formatDate(new Date(), " dddd d MMMM yyyy")
        }

        Text {
            id: timeText

            anchors {
                right: parent.right; rightMargin: platformStyle.paddingSmall
                verticalCenter: parent.verticalCenter
            }

            color: platformStyle.colorNormalLight
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeMedium; weight: Font.Bold }
            text: symbian.currentTime

            Behavior on opacity { PropertyAnimation { duration: 200 } }
        }

        states: [
            State {
                name: "Edit"; when: inputContext.visible
                AnchorChanges { target: dateText; anchors.left: parent.left }
                PropertyChanges { target: timeText; opacity: 1 }
            },

            State {
                name: "View"; when: !inputContext.visible
                AnchorChanges { target: dateText; anchors.left: undefined; anchors.horizontalCenter: parent.horizontalCenter }
                PropertyChanges { target: timeText; opacity: 0 }
            }
        ]

        transitions: [ Transition { AnchorAnimation { duration: 200 } } ]
    }

//! [0]
    TextArea {
        id: textArea

        anchors {
            top: filler.bottom; bottom: splitViewInput.top
            left: parent.left; right: parent.right;
        }

        placeholderText: "Tap to write"

        Behavior on height { PropertyAnimation { duration: 200 } }
    }

    Item {
        id: splitViewInput

        anchors { bottom: parent.bottom; left: parent.left; right: parent.right }

        Behavior on height { PropertyAnimation { duration: 200 } }

        states: [
            State {
                name: "Visible"; when: inputContext.visible
                PropertyChanges { target: splitViewInput; height: inputContext.height }
            },

            State {
                name: "Hidden"; when: !inputContext.visible
                PropertyChanges { target: splitViewInput; height: toolBar.height }
            }
        ]
    }
//! [0]

//! [2]
    ToolBar {
        id: toolBar

        anchors { bottom: parent.bottom }

        opacity: !inputContext.visible

        Behavior on opacity { PropertyAnimation { duration: 200 } }

        tools: ToolBarLayout {

            ToolButton { iconSource: "toolbar-back"; onClicked: Qt.quit() }

            ButtonRow {
                exclusive: false
                ToolButton { iconSource: "toolbar-previous" }
                ToolButton { iconSource: "toolbar-share" }
                ToolButton { iconSource: "toolbar-delete" }
                ToolButton { iconSource: "toolbar-next" }
            }
        }
    }
//! [2]

}
