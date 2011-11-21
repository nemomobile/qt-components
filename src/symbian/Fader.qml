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
import "RectUtils.js" as RectUtils

Item {
    id: root

    property real dimm: 0.8
    property int animationDuration: 500
    property Item visualParent: null
    property bool platformInverted: false // supports inverted, but looks the same
    property variant popupRect: Qt.rect(0, 0, 0, 0)

    signal clicked

    anchors.fill: parent
    anchors.bottomMargin: inputContext.visible ? inputContext.height : 0
    state: "Hidden"

    Rectangle {
        id: fader

        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: parent.height + parent.anchors.bottomMargin
        opacity: 0.0
        color: "black"

        //eat mouse events
        MouseArea {
            id: mouseEventEater
            anchors.fill: parent
            enabled: fader.opacity != 0.0
            preventStealing: true

            onPressed: {
                root.state = "Pressed"
                mouse.accepted = true
            }
            onReleased: {
                if (root.state == "Pressed")
                    root.clicked()
                else
                    root.state = "Visible"
            }
            onPositionChanged: {
                if (RectUtils.rectContainsPoint(popupRect, mouseX, mouseY)
                        && root.state == "Pressed")
                    root.state = "Visible"
            }
        }
    }

    states: [
        State { name: "Visible" },
        State { name: "Hidden" },
        State { name: "Pressed" }
    ]

    transitions: Transition {
        from: "Hidden"
        to: "Visible"
        reversible: true
        PropertyAnimation {
            target: fader; property: "opacity"; to: dimm; duration: animationDuration; easing.type: Easing.Linear }
    }
}
