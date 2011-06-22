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

FocusScope {
    anchors.fill: parent
    property variant focusItem: textFields.activeFocus ? textFields : textAreas.activeFocus
                                                       ? textAreas  : textInputs.activeFocus
                                                       ? textInputs : null
    focus: true
    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Component {
        id: highlight
        Rectangle {
            border { color: "steelblue"; width: 4 }
            color: "#00000000"; radius: 5
            height: focusItem ? focusItem.currentItem.height : 0
            width: focusItem ? focusItem.currentItem.width : 0
            y: focusItem ? focusItem.currentItem.y : 0; z: 5
            Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
            visible: ListView.view.activeFocus
        }
    }

    Rectangle {
        border { color: "hotpink"; width: 4 }
        color: "#00000000"; radius: 5; opacity: 0.80
        x: focusItem ? focusItem.x + container.anchors.margins / 2: 0
        y: focusItem ? focusItem.y + container.anchors.margins / 2: 0
        height: focusItem ? focusItem.height + container.anchors.margins : 0
        width: focusItem ? focusItem.width + container.anchors.margins : 0
        visible: focusItem ? focusItem.activeFocus : false
        Behavior on y { SpringAnimation { spring: 1; damping: 0.1 } }
    }

    Column {
        id: container

        property real h: (height - spacing * (children.length - 1)) / children.length

        anchors { fill: parent; margins: 10 }
        spacing: 10

        ListView {
            id: textFields

            delegate: TextField {
                text: name; placeholderText: "Enter Name"; width: textFields.width
                onActiveFocusChanged: if (activeFocus) textFields.currentIndex = index
            }

            model: ListModel {
                ListElement {name: "Bob"}
                ListElement {name: "John"}
                ListElement {name: "Michael"}
                ListElement {name: ""}
                ListElement {name: "Greg"}
                ListElement {name: "Eric"}
                ListElement {name: "Oliver"}
                ListElement {name: "Jack"}
            }

            clip: true; spacing: 5
            height: parent.h; width: parent.width
            highlight: highlight
            highlightFollowsCurrentItem: false
            focus: true
            KeyNavigation.down: textAreas
        }

        ListView {
            id: textAreas

            delegate: TextArea {
                text: name; placeholderText: "Enter Text"; width: textAreas.width
                onActiveFocusChanged: if (activeFocus) textAreas.currentIndex = index
            }

            model: ListModel {
                ListElement {name: "This is a really long piece of text."}
                ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. "}
                ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet."}
                ListElement {name: ""}
                ListElement {name: "This is a really long piece of text. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam tristique augue ac mauris cursus vel porta tortor aliquet. Nam tristique augue ac mauris cursus vel porta tortor aliquet."}
            }

            clip: true; spacing: 5
            height: parent.h; width: parent.width
            highlight: highlight
            highlightFollowsCurrentItem: false
            KeyNavigation.up: textFields; KeyNavigation.down: textInputs
        }

        ListView {
            id: textInputs

            delegate: FocusScope {
                anchors.margins: 5
                width: textInputs.width; height: textInput.font.pixelSize + anchors.margins

                Rectangle {
                    anchors.fill: parent
                    color: "khaki"; radius: 5
                }

                TextInput {
                    id: textInput
                    anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                    color: "black"; focus: true; text: name; font.pixelSize: 18
                    onActiveFocusChanged: if (activeFocus) textInputs.currentIndex = index
                }
            }

            model: ListModel {
                ListElement {name: "Bob"}
                ListElement {name: "John"}
                ListElement {name: "Michael"}
                ListElement {name: ""}
                ListElement {name: "Greg"}
                ListElement {name: "Oliver"}
                ListElement {name: "Jack"}
            }

            clip: true
            height: parent.h; width: parent.width; spacing: 5
            highlight: highlight
            highlightFollowsCurrentItem: false
            KeyNavigation.up: textAreas
        }
    }
}
