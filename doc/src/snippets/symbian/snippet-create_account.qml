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
import QtQuick 1.0
import com.nokia.symbian 1.1

Window {
    id: window

    StatusBar {
        id: statusBar
        anchors { left: parent.left; right: parent.right }
        state: inputContext.visible ? "Hidden" : "Visible"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: statusBar; y: 0; opacity: 1 }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: statusBar; y: -height; opacity: 0 }
            }
        ]

        transitions: [
            Transition {
                from: "Hidden"; to: "Visible"
                ParallelAnimation {
                    NumberAnimation { target: statusBar; properties: "y"; duration: 200; easing.type: Easing.OutQuad }
                    NumberAnimation { target: statusBar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            },
            Transition {
                from: "Visible"; to: "Hidden"
                ParallelAnimation {
                    NumberAnimation { target: statusBar; properties: "y"; duration: 200; easing.type: Easing.Linear }
                    NumberAnimation { target: statusBar; properties: "opacity"; duration: 200; easing.type: Easing.Linear }
                }
            }
        ]
    }

    ListView {
        id: listView

        anchors {
            top: statusBar.bottom;
            left: parent.left;
            right: parent.right;
            bottom: toolBar.top
            margins: platformStyle.paddingSmall
        }

        clip: true; spacing: platformStyle.paddingSmall

        delegate: TextField {
            placeholderText: placeHolder
            width: listView.width
            onActiveFocusChanged: if (activeFocus) listView.currentIndex = index
        }

        model: ListModel {
            ListElement { placeHolder: "First Name" }
            ListElement { placeHolder: "Last Name" }
            ListElement { placeHolder: "Nick Name" }
            ListElement { placeHolder: "Phone Number" }
            ListElement { placeHolder: "Alternative Phone Number" }
            ListElement { placeHolder: "Email" }
            ListElement { placeHolder: "Password" }
            ListElement { placeHolder: "Verify Password" }
            ListElement { placeHolder: "Description" }
            ListElement { placeHolder: "Company" }
            ListElement { placeHolder: "Street Address" }
            ListElement { placeHolder: "Postal Code" }
            ListElement { placeHolder: "Country" }
        }

        onCurrentItemChanged: if (currentItem.activeFocus) currentItem.openSoftwareInputPanel()
    }

    ToolBar {
        id: toolBar

        anchors {
            left: parent.left;
            right: parent.right;
            bottom: vkb.top
        }

        tools: ToolBarLayout {
            ToolButton {
                iconSource: "toolbar-back"
                visible: !vkb.visible
                onClicked: Qt.quit()
            }

            ButtonRow {
                exclusive: false
                visible: vkb.visible

                ToolButton {
                    iconSource: "toolbar-mediacontrol-backwards"
                    onClicked: listView.decrementCurrentIndex()
                }

                ToolButton {
                    iconSource: "toolbar-mediacontrol-forward"
                    onClicked: listView.incrementCurrentIndex()
                }

                ToolButton { iconSource: "toolbar-add" }
            }
        }
    }

    Item {
        id: vkb

        anchors {
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }

        visible: inputContext.visible
        state: visible ? "Visible" : "Hidden"

        states: [
            State {
                name: "Visible"
                PropertyChanges { target: vkb; height: inputContext.height; }
            },
            State {
                name: "Hidden"
                PropertyChanges { target: vkb; height: 0;}
            }
        ]

        transitions: [
            Transition {
                from: "Hidden"; to: "Visible"
                NumberAnimation { target: vkb; properties: "height"; duration: 200; easing.type: Easing.OutQuad }
            },
            Transition {
                from: "Visible"; to: "Hidden"
                NumberAnimation { target: vkb; properties: "height"; duration: 200; easing.type: Easing.Linear }
            }
        ]
    }
}
