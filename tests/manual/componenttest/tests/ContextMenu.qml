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
    ContextMenu {
        id: menu

        content: MenuLayout {
            MenuItem {text: "Zero"; onClicked: signalLabel.text = "itemClicked(0)"}
            MenuItem {text: "One"; onClicked: signalLabel.text = "itemClicked(1)"}
            MenuItem {text: "Two"; onClicked: signalLabel.text = "itemClicked(2)"}
            MenuItem {text: "Three"; onClicked: signalLabel.text = "itemClicked(3)"}
            MenuItem {text: "Four"; onClicked: signalLabel.text = "itemClicked(4)"}
            MenuItem {text: "Five"; onClicked: signalLabel.text = "itemClicked(5)"}
            MenuItem {text: "Six"; onClicked: signalLabel.text = "itemClicked(6)"}
            MenuItem {text: "Seven"; onClicked: signalLabel.text = "itemClicked(7)"}
            MenuItem {text: "Eight"; onClicked: signalLabel.text = "itemClicked(8)"}
            MenuItem {text: "Nine"; onClicked: signalLabel.text = "itemClicked(9)"}
            MenuItem {text: "Ten"; onClicked: signalLabel.text = "itemClicked(10)"}
            MenuItem {text: "Eleven"; onClicked: signalLabel.text = "itemClicked(11)"}
            MenuItem {text: "Twelve"; onClicked: signalLabel.text = "itemClicked(12)"}
            MenuItem {text: "Thirteen"; onClicked: signalLabel.text = "itemClicked(13)"}
            MenuItem {text: "Fourteen"; onClicked: signalLabel.text = "itemClicked(14)"}
            MenuItem {text: "Fifteen"; onClicked: signalLabel.text = "itemClicked(15)"}
        }
    }

    Rectangle {
        id: triggerArea
        color: "lightblue"
        x: 50
        y: 20
        width: 200
        height: 100

        Label {
            text: "Press and hold here"
            anchors.centerIn: parent
            color: "black"
        }

        MouseArea {
            anchors.fill: parent
            onPressAndHold: {
                signalLabel.text = "No signal"
                menu.open()
            }
        }
    }

    Rectangle {
        id: triggerArea2
        color: "yellow"
        x: 50
        y: 120
        width: 200
        height: 100

        Label {
            text: "Press and hold here"
            anchors.centerIn: parent
            color: "black"
        }

        MouseArea {
            anchors.fill: parent

            onPressAndHold: {
                signalLabel.text = "No signal"
                menu.open()
                menuDismissTimer.start()
            }
        }
    }

    Timer {
        id: menuDismissTimer
        interval: 2000
        onTriggered: menu.close()
    }

    Row {
        anchors { top: triggerArea2.bottom; topMargin: 20 }
        x: 50

        Label {
            text: "Signal: "
        }

        Label {
            id: signalLabel
            objectName: "signalLabel"
        }
    }
}
