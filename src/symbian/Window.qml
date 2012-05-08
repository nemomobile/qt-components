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
import "." 1.1

Rectangle {
    id: root

    property bool inPortrait: window.height > window.width
    default property alias content: window.data
    property bool platformInverted: false
    property Item privateWindow: window

    width: screen.privateSensorOrientationMethod() ? screen.displayWidth : screen.width
    height: screen.privateSensorOrientationMethod() ? screen.displayHeight : screen.height

    signal orientationChangeAboutToStart
    signal orientationChangeStarted
    signal orientationChangeFinished
    signal privateSwitchGeometry

    color: platformInverted ? platformStyle.colorBackgroundInverted
                            : platformStyle.colorBackground

    Snapshot {
        id: snapshot

        property int angle: 0
        property int statAngle: 0

        anchors.top: parent.top
        anchors.left: parent.left
        width: screen.displayWidth
        height: screen.displayHeight
        snapshotWidth: screen.displayWidth
        snapshotHeight: screen.displayHeight
        opacity: 0
        smooth: true
    }

    Item {
        id: window
        objectName: "window"
        rotation: -screen.rotation

        function setOrientationWithoutAnimation() {
            root.orientationChangeStarted();
            root.privateSwitchGeometry()
            window.rotation = snapshot.angle;
            root.orientationChangeFinished();
        }

        // Modulo function that works with negative numbers
        function mod(a, n) {
            return ((a % n) + n) % n;
        }

        anchors.centerIn: parent
        clip: orientationAnimation.running
        smooth: !orientationAnimation.running

        Binding { target: window; property: "height"; value: screen.height; when: screen.height > 0 }
        Binding { target: window; property: "width"; value: screen.width; when: screen.width > 0 }

        Timer {
            id: asyncOrientation

            property bool animate: false

            triggeredOnStart:  false
            interval: 1
            repeat: false
            onTriggered: {
                if (asyncOrientation.animate && !inputContext.visible && Qt.application.active)
                    orientationAnimation.restart();
                 else
                    window.setOrientationWithoutAnimation();
            }
        }

        Connections {
            target: screen
            onPrivateAboutToChangeOrientation: {
                if (animate && (window.mod(angle, 360) == window.mod(window.rotation, 360)))
                    return;

                orientationAnimation.complete();
                snapshot.angle = angle;
                snapshot.statAngle = window.rotation;
                asyncOrientation.animate = animate;
                root.orientationChangeAboutToStart();

                if (animate)
                    asyncOrientation.restart();
                else
                    window.setOrientationWithoutAnimation();
            }
        }

        SequentialAnimation {
            id: orientationAnimation;

            ScriptAction { script: { snapshot.take() } }
            PropertyAction { target: snapshot; property: "opacity"; value: 1.0 }
            PropertyAction { target: window; property: "opacity"; value: 0.0 }
            ScriptAction { script: { root.orientationChangeStarted() } }

            ParallelAnimation {
                PropertyAnimation { target: snapshot; properties: "scale"; to: 0.9; duration: 400 }
                ScriptAction { script: {root.privateSwitchGeometry() } }
                PropertyAction { target: window; property: "rotation"; value: snapshot.angle }
                PropertyAction { target: window; property: "scale"; value: 0.9 }
            }

            PropertyAction { target: snapshot; property: "opacity"; value: 0.0 }
            PropertyAction { target: snapshot; property: "scale"; value: 1.0 }
            PropertyAction { target: window; property: "opacity"; value: 1.0 }
            ScriptAction { script: { snapshot.take() } }
            PropertyAction { target: snapshot; property: "opacity"; value: 1.0 }
            PropertyAction { target: window; property: "opacity"; value: 0.0 }

            ParallelAnimation {
                RotationAnimation {
                    target: snapshot;
                    from: snapshot.statAngle - snapshot.angle;
                    to: 0;
                    direction: RotationAnimation.Shortest;
                    duration: 400;
                    easing.type: Easing.OutQuad
                }
                PropertyAnimation { target: snapshot; properties: "scale"; from:1; to: 1.1; duration: 400 }
            }

            PropertyAction { target: window; property: "scale"; value: 1.0 }
            PropertyAction { target: snapshot; property: "opacity"; value: 0.0 }
            PropertyAction { target: snapshot; property: "scale"; value: 1.0 }
            PropertyAction { target: snapshot; property: "rotation"; value: 0.0 }
            ScriptAction { script: { snapshot.free() } }
            PropertyAction { target: window; property: "opacity"; value: 1.0 }
            ScriptAction { script: { root.orientationChangeFinished() } }
        }
    }
}
