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
import Qt.labs.components 1.0

Item {
    id: root

    property variant focusItem: addText.activeFocus ? addText
                              : clearText.activeFocus ? clearText
                              : preText.activeFocus ? preText
                              : postText.activeFocus ? postText
                              : null

    Component.onCompleted: addText.forceActiveFocus()

    Rectangle {
        border {color: "steelblue"; width: 5}
        color: "#00000000"; radius: 5; opacity: 0.80
        x: focusItem ? focusItem.x + column.anchors.margins : 0
        y: focusItem ? focusItem.y + column.anchors.margins : 0
        height: focusItem ? focusItem.height + focusItem.anchors.margins : 0
        width: focusItem ? focusItem.width + focusItem.anchors.margins * 2 : 0
        visible: focusItem ? focusItem.activeFocus : false
    }

    Column {
        id: column
        anchors {fill: parent; margins: 20}
        spacing: 10

        Grid {
            id: buttons

            property real bh: (height - spacing * (rows - 1)) / rows
            property real bw: (width - spacing * (columns - 1)) / columns

            columns: 2; rows: 2; spacing: 10
            width: parent.width; height: parent.height / 3

            Button {
                id: addText
                objectName: "addText"
                width: parent.bw; height: parent.bh
                text: "Add text"

                KeyNavigation.right: clearText
                KeyNavigation.down: preText

                onClicked: editor.text = (preText.checked ? "Toggled:" : "")
                            + "Lorem ipsum" + (postText.checked ? ":Toggled\n" : "\n") + editor.text
            }

            Button {
                id: clearText
                objectName: "clearText"
                text: "Clear text"
                width: parent.bw; height: parent.bh

                KeyNavigation.left: addText
                KeyNavigation.down: postText

                onClicked: editor.text = ""
            }

            Button {
                id: preText
                objectName: "preText"
                text: "Toggle start"
                checkable: true
                width: parent.bw; height: parent.bh

                KeyNavigation.up: addText
                KeyNavigation.right: postText
                KeyNavigation.down: one
            }

            Button {
                id: postText
                objectName: "postText"
                text: "Toggle end"
                checkable: true
                width: parent.bw; height: parent.bh

                KeyNavigation.up: clearText
                KeyNavigation.left: preText
                KeyNavigation.down: three
            }
        }

        TextArea {
            id: editor
            objectName: "editor"
            width: parent.width; height: parent.height / 3
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: true
            enabled: false
        }

        Row {
            id: radioButtons

            property real bw: width / children.length

            width: parent.width; height: parent.height / 3

            CheckableGroup { id: group }

            RadioButton {
                id: one
                objectName: "one"
                platformExclusiveGroup: group
                width: parent.bw
                text: "one"

                KeyNavigation.up: preText
                KeyNavigation.right: two
            }
            RadioButton {
                id: two
                objectName: "two"
                platformExclusiveGroup: group
                width: parent.bw
                text: "two"

                KeyNavigation.up: preText
                KeyNavigation.left: one
                KeyNavigation.right: three
            }
            RadioButton {
                id: three
                objectName: "three"
                platformExclusiveGroup: group
                width: parent.bw
                text: "three"

                KeyNavigation.up: postText
                KeyNavigation.left: two
            }
        }
    }
}
