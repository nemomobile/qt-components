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
import "../components"

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

        Label {
            id: horizontalBarValue

            text: "ProgressBar value: " + horizontalBar.value
            height: 20
            width: 50
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

        Button {
            id: indeterminateButton

            text: "Indeterminate"
            checkable: true
            checked: false
            onCheckedChanged: horizontalBar.indeterminate = checked
        }
    }
}
