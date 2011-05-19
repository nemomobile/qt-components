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
import "ScalableUI.js" as ScalableUI

Window {
    id: window

    //![0]
    Box {
        id: greenBox
        width: ScalableUI.greenBoxWidth
        height: ScalableUI.greenBoxHeight
        anchors.top:  parent.top
        borderImage: "green.png"
    }

    Box {
        id: blueBox
        height: ScalableUI.blueBoxHeight
        anchors.top: parent.top
        anchors.left: greenBox.right
        anchors.right: parent.right
        borderImage: "blue.png"
    }

    Box {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: blueBox.bottom
        anchors.bottom: parent.bottom
        borderImage: "red.png"
    }
    //![0]

    states: [
        //![1]
        State {
            name: "portraitLayout"
            when: window.inPortrait
            AnchorChanges {
                target: greenBox
                anchors.left: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }

            AnchorChanges {
                target: blueBox
                anchors.top: greenBox.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }
        },

        State {
            name: "landscapeLayout"
            when: !window.inPortrait
            AnchorChanges {
                target: greenBox
                anchors.left: parent.left
                anchors.horizontalCenter: undefined
            }

            AnchorChanges {
                target: blueBox
                anchors.top: parent.top
                anchors.left: greenBox.right
                anchors.right: parent.right
            }
        }
        //![1]
    ]

    transitions: Transition {
        // smoothly reanchor between layouts
        AnchorAnimation { duration: 1000; easing.type: Easing.OutBounce }
    }
}
