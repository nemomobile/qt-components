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
// Note: This example imports the “Qt.labs.components.native 1.0" module that allows the same
// Qt Quick Components example code to run as is in both MeeGo 1.2 Harmattan and Symbian platforms
// during the application development. However, real published applications should not import this
// module but one of the following platform-specific modules instead:
// import com.nokia.symbian 1.1    // Symbian components
// import com.nokia.meego 1.1      // MeeGo components
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
