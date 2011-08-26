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

Item {
    id: root
    property bool platformInverted: false

    Component {
        id: longText
        ToolTip {
            text: "ToolTip appears. Text is elided if it doesn't fit in the ToolTip."
            objectName: "longText"
            target: topLeft
            platformInverted: root.platformInverted
        }
    }

    Component {
        id: richText
        ToolTip {
            text: "<b style=color:'steelblue'><i>RichText ToolTip</b><i>"
            objectName: "richText"
            target: topRight
            platformInverted: root.platformInverted
        }
    }

    Button {
        id: fullScreen
        text: "Toggle fullscreen"
        objectName: "fullScreen"
        anchors.centerIn: parent
        width: 200

        onClicked: {
            mainWindow.fullScreen = !mainWindow.fullScreen
        }
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
        text: doubleWord.visible ? "Hide" : "Show"
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

        onClicked: doubleWord.visible = !doubleWord.visible
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
        text: singleWord.visible ? "Hide" : "Show"
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

        onClicked: singleWord.visible ? singleWord.hide() : singleWord.show()
    }
}
