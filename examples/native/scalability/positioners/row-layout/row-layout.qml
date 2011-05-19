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
import Qt.labs.components.native 1.0

Window {
    id: window

    Row {
        id: row

        //![0]
        property int itemCount: window.inPortrait ? 2 : 4
        property int rectangleWidth: Math.round(width / itemCount)
        //![0]

        anchors.fill: parent
        anchors.margins:  1

        move: Transition {
            NumberAnimation { properties: "x"; easing.type: Easing.OutBounce }
        }
        add: Transition {
            NumberAnimation { properties: "x"; easing.type: Easing.OutQuad }
        }

        //![1]
        Repeater {
            model: ["red", "lightsteelblue", "green", "orange"]

            Rectangle {
                width: row.rectangleWidth; height: row.height
                color: modelData
                border.color: "white"
                radius: 15
            }
        }
        //![1]
    }

    Text {
        anchors.centerIn: parent
        font.pointSize: 14
        text: "Number of items " + row.itemCount
    }
}
