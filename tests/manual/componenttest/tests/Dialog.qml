/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import com.nokia.symbian 1.0

Item {
    Dialog {
        id: dialog

        title: Text {
            text: "Dialog"
            font { bold: true; pixelSize: 16 }
            color: "white"
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        buttons: Row {
            height: 60
            width: parent.width
            Button {
                text: "Ok"
                width: parent.width / 2
                height: parent.height
                onClicked: dialog.accept()
            }

            Button {
                text: "Cancel"
                width: parent.width / 2
                height: parent.height
                onClicked: dialog.reject()
            }
        }
        content: Text {
            text: "This is content text."
            font.bold: true
            font.pixelSize: 18
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Dialog {
        id: dialogWithProgressBar

        width: 400
        height: 200
        title: Text {
            text: "Dialog with Progress"
            font { bold: true; pixelSize: 16 }
            color: "white"
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        buttons: Row {
            height: 60
            width: parent.width
            Button {
                text: "Ok"
                width: parent.width
                height: parent.height
                onClicked: dialogWithProgressBar.accept()
            }
        }
        content: Item {
            height: 250
            x: 20
            y: 20

            ProgressBar {
                id: horizontalBarWithAnimation

                value: 1
                indeterminate: true
                x: 0
                y: 0
                height: 30
                width: 180
            }

            ProgressBar {
                id: horizontalBar

                value: 75 // Default minimum and maximum are 0 - 100
                x: 0
                y: 30
                height: 30
                width: 180
            }
        }
    }

    Dialog {
        id: dialogWithScrollBar

        title: Row {
            width: parent.width
            Text {
                width: parent.width - closeIcon.width
                text: "Dialog with Scrolling"
                font { bold: true; pixelSize: 16 }
                color: "white"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                id: closeIcon

                radius: 5
                width: 25
                height: 25
                color: "grey"
                MouseArea {
                    anchors.fill: parent
                    onClicked: dialogWithScrollBar.reject()
                }
            }
        }

        content: Item {
            id: page

            anchors.fill: parent
            Flickable {
                id: flickableArea

                anchors.fill: parent
                contentWidth: colorfulSquare.width
                contentHeight: colorfulSquare.height
                clip: true

                Rectangle {
                    id: colorfulSquare

                    width: 1000
                    height: 1000
                    opacity: 0.5
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "blue" }
                        GradientStop { position: 1.0; color: "yellow" }
                    }
                }
                Rectangle {
                    height: colorfulSquare.height
                    width: colorfulSquare.width
                    opacity: 0.5
                    rotation: -90
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "red" }
                        GradientStop { position: 1.0; color: "green" }
                    }
                }
                Text {
                    anchors.centerIn: parent
                    text: "Flick around!"
                    color: "white"
                    font.pixelSize: 50
                    opacity: 0.85
                }
            }
            ScrollBar {
                id: verticalScrollbar

                orientation: Qt.Vertical
                flickableItem: flickableArea
                anchors { top: flickableArea.top; right: flickableArea.right }
            }
            ScrollBar {
                orientation: Qt.Horizontal
                flickableItem: flickableArea
                anchors {
                    bottom: flickableArea.bottom
                    left: flickableArea.left
                    rightMargin: verticalScrollbar.width
                }
            }
        }
    }

    Button {
        x: 10
        y: 50
        width: 160
        height: 50
        text: "Show Dialog"

        onClicked: dialog.open()
    }

    Button {
        id: button2
        x: 10
        y: 150
        width: 160
        height: 50
        text: "Show Dialog 2"

        onClicked: dialogWithProgressBar.open()
    }

    Button {
        x: 10
        y: 250
        width: 160
        height: 50
        text: "Show Dialog 3"

        onClicked: dialogWithScrollBar.open()
    }
}
