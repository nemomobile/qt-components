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

import QtQuick 1.0
import "../../../" as Root

Rectangle {
    id: rect

    property int buttonSide: rect.width / 3
    property string sourceInfo: "layouts/Normal/High/MainView.qml"
    property string displayInfo: Math.max(rect.width, rect.height) + "x"
                                 + Math.min(rect.width, rect.height) + ", "
                                 + Math.round(screen.dpi) + " DPI"

    anchors.fill: parent
    color: "black"

    Root.GradientRectangle {
        id: output

        color1: "white"
        color2: "blue"
        width: rect.width
        height: rect.height / 4 * 3
        anchors { top: rect.top; bottom: button1.top }

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            text: rect.sourceInfo + "\n" + rect.displayInfo
        }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = "blue" }
    }

    Root.GradientRectangle {
        id: button1

        color1: "lightgreen"
        color2: "green"
        width: rect.buttonSide
        height: width
        anchors { bottom: rect.bottom; left: rect.left }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    Root.GradientRectangle {
        id: button2

        color1: "white"
        color2: "yellow"
        width: rect.buttonSide
        height: width
        anchors { bottom: rect.bottom; horizontalCenter: rect.horizontalCenter }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    Root.GradientRectangle {
        id: button3

        color1: "white"
        color2: "red"
        width: rect.buttonSide
        height: width
        anchors { bottom: rect.bottom; right: rect.right }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    //![0]
    states: State {
        name: "Landscape"
        when: screen.width > screen.height

        AnchorChanges {
            target: output
            anchors {
                left: rect.left
                right: button1.left
                bottom: rect.bottom
            }
        }

        AnchorChanges {
            target: button1
            anchors {
                top: rect.top
                bottom: undefined
                left: undefined
                right: rect.right
            }
        }

        AnchorChanges {
            target: button2
            anchors {
                verticalCenter: rect.verticalCenter
                horizontalCenter: undefined
                right: rect.right
                bottom: undefined
            }
        }

        PropertyChanges {
            target: output
            width: rect.width / 4 * 3
            height: rect.height
        }

        PropertyChanges {
            target: rect
            buttonSide: rect.height / 3
        }
    }
    //![0]
}
