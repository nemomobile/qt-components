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
    ObjectMenu {
        id: menu

        title: "Object menu title"
        actions: ["Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"]

        onTriggered: {
            console.log("Item " + index + " selected")
            signalLabel.text = "itemClicked(" + index + ")"
        }

        onCanceled: {
            console.log("Menu canceled")
            signalLabel.text = "canceled"
        }
    }

    Rectangle {
        id: triggerArea

        color: "lightblue"
        x: 50
        y: 20
        width: 200
        height: 100

        Text {
            text: "Press and hold here"
            anchors.centerIn: parent
            font { bold: true; pixelSize: 14 }
        }

        MouseArea {
            anchors.fill: parent
            onPressAndHold: menu.open()
        }
    }

    Rectangle {
        id: triggerArea2

        color: "yellow"
        x: 50
        y: 120
        width: 200
        height: 100

        Text {
            text: "Press and hold here"
            anchors.centerIn: parent
            font { bold: true; pixelSize: 14 }
        }

        MouseArea {
            anchors.fill: parent

            onPressAndHold: {
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

        Text {
            font { bold: true; pixelSize: 14 }
            color: "white"
            text: "Signal: "
        }

        Text {
            id: signalLabel

            objectName: "signalLabel"
            font { bold: true; pixelSize: 14 }
            color: "white"
        }
    }
}
