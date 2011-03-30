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
import Qt 4.7
import com.nokia.symbian 1.0

FocusScope {
    anchors.fill: parent
    property variant focusItem: textFields.activeFocus ? textFields : textAreas.activeFocus
                                                       ? textAreas  : textInputs.activeFocus
                                                       ? textInputs : null
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
            highlight: activeFocus ? highlight : null
            highlightFollowsCurrentItem: false
            Component.onCompleted: forceActiveFocus()
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
            highlight: activeFocus ? highlight : null
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
            highlight: activeFocus ? highlight : null
            highlightFollowsCurrentItem: false
            KeyNavigation.up: textAreas
        }
    }
}
