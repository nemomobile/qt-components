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

Item {

    Column {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 20

        ProgressBar {
            id: horizontalBarWithAnimation

            width: parent.width
            indeterminate: true

            Rectangle {
                z: -1
                anchors.fill: parent
                color: "#00000000"
                border.color: "red"
            }

            Rectangle {
                width: parent.implicitWidth
                height: parent.implicitHeight
                anchors.centerIn: parent
                color: "#00000000"
                border.color: "blue"
            }
        }

        Text {
            id: horizontalBarValue

            text: "ProgressBar value: " + horizontalBar.value
            height: 20
            width: 50
            color: "white"
        }

        ProgressBar {
            id: horizontalBar

            objectName: "horizontalBar"
            width: parent.width
            value: 1 // Default minimum and maximum are 0.0 and 1.0

            state: "full"
            states: [
                State {
                    name: "empty"
                    PropertyChanges { target: horizontalBar; value: 0 }
                },
                State {
                    name: "full"
                    PropertyChanges { target: horizontalBar; value: maxProgressValue.text }
                }
            ]

            transitions: [
                Transition {
                    PropertyAnimation { target: horizontalBar; properties: "value"; duration: 1000 }
                }
            ]
        }

        TextField {
            id: maxProgressValue

            objectName: "maxProgressValue"
            width: 50
            height: 40
            text: "1.0"
        }

        Button {
            id: button

            text: "Min / Restore"
            onClicked: horizontalBar.state == "full" ? horizontalBar.state = "empty" : horizontalBar.state = "full"
        }
    }
}
