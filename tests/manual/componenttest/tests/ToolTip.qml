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
import com.nokia.symbian 1.0

Item {
    id: root

    Component {
        id: longText
        ToolTip {
            text: "ToolTip appears. Text is elided if it doesn't fit in the ToolTip."
            objectName: "longText"
            target: topLeft
        }
    }

    Component {
        id: richText
        ToolTip {
            text: "<b style=color:'steelblue'><i>RichText ToolTip</b><i>"
            objectName: "richText"
            target: topRight
        }
    }

    Button {
        id: fullScreen
        text: "Toggle fullscreen"
        objectName: "fullScreen"
        anchors.centerIn: parent
        width: 200

        onClicked: mainWindow.fullScreen = !mainWindow.fullScreen;
    }

    Button {
        id: topLeft
        text: "Press"
        objectName: "topLeft"
        width: 125

        anchors {top: parent.top; left: parent.left}

        Loader {
            sourceComponent: topLeft.pressed ? longText : undefined
            onLoaded: {
                visible = false
                visible = true
            }
        }
    }

    Button {
        id: bottomLeft
        text: "Show"
        objectName: "bottomLeft"
        width: 125

        anchors {bottom: parent.bottom; left: parent.left}

        ToolTip {
            id: doubleWord
            text: "ToolTip ToolTip"
            objectName: "doubleWord"
            target: bottomLeft
            visible: false
        }

        onClicked: {
            doubleWord.visible = !doubleWord.visible
            bottomLeft.text  = doubleWord.visible ? "Hide" : "Show"
        }
    }

    Button {
        id: topRight
        text: "Press"
        objectName: "topRight"
        width: 125

        anchors {top: parent.top; right: parent.right}

        Loader {
            sourceComponent: topRight.pressed ? richText : undefined
            onLoaded: {
                visible = false
                visible = true
            }
        }
    }

    Button {
        id: bottomRight
        text: "Show"
        objectName: "bottomRight"
        width: 125

        anchors {bottom: parent.bottom; right: parent.right}

        ToolTip {
            id: singleWord
            text: "Tip!"
            objectName: "bottomRightToolTip"
            target: bottomRight
            visible: false
        }

        onClicked: {
            singleWord.visible = !singleWord.visible
            bottomRight.text  = singleWord.visible ? "Hide" : "Show"
        }
    }
}
