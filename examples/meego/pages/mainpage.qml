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

import Qt 4.7
import com.meego 1.0

Page {
    id: page
    property int num: 1
    property string idx_prefix: ""

    Rectangle {
        anchors.fill: parent

        property real amountOfR: Math.random()
        property real amountOfG: Math.random()
        property real amountOfB: Math.random()
        color: Qt.rgba(amountOfR, amountOfG, amountOfB, 1)
    }

    Text {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "Page number " + page.num
    }

    Column {
        anchors.centerIn: parent

        Repeater {
            model: 8
            Rectangle {
                height: 50
                width: 600

                border.color: "black"
                border.width: 3

                Text {
                    id: text
                    anchors.centerIn: parent
                    text: "Click me, I am index: " + idx_prefix + index
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!window.nextPage(window.mainpage))
                            return
                        window.currentPage.num = num + 1
                        window.currentPage.idx_prefix = idx_prefix + index + "->"
                    }
                }
            }
        }
    }

    Button {
        text: "rotate"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 100
        height: 30
        onClicked: {
            if (window.state == "Portrait") {
                window.state = "Landscape";
            } else if (window.state == "Landscape") {
                window.state = "PortraitInverted";
            } else if (window.state == "PortraitInverted") {
                window.state = "LandscapeInverted";
            } else {
                window.state = "Portrait";
            }
        }
    }
}
