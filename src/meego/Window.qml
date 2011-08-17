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
import "." 1.0

Item {
    id: root
    width: screen.displayWidth
    height: screen.displayHeight

    property alias color: background.color

    default property alias content: windowContent.data

    // Read only property true if window is in portrait
    property alias inPortrait: window.portrait

    signal orientationChangeAboutToStart
    signal orientationChangeStarted
    signal orientationChangeFinished

    Rectangle {
        id: background
        anchors.fill: parent
        color: "black"
    }

    Item {
        id: window
        property bool portrait

        width: window.portrait ? screen.displayHeight : screen.displayWidth
        height: window.portrait ? screen.displayWidth : screen.displayHeight

        anchors.left : parent.left
        anchors.top : parent.top
        transform: [ Translate { id: windowTranslate; x: 0; y: 0 },
                     Rotation { id: windowRotation;
                                origin.x: screen.displayHeight / 2; origin.y: screen.displayHeight / 2;
                                angle: 0 } ]

        Item {
            id: windowContent
            width: parent.width
            height: parent.height - heightDelta

            // Used for resizing windowContent when virtual keyboard appears
            property int heightDelta: 0

            objectName: "windowContent"
            clip: true

            Connections {
                id: inputContextConnection
                target: inputContext
                onSoftwareInputPanelVisibleChanged: inputContextConnection.updateWindowContentHeightDelta();

                onSoftwareInputPanelRectChanged: inputContextConnection.updateWindowContentHeightDelta();

                function updateWindowContentHeightDelta() {
                    if(inputContext.customSoftwareInputPanelVisible)
                        return

                    if (root.inPortrait)
                        windowContent.heightDelta = inputContext.softwareInputPanelRect.width
                    else
                        windowContent.heightDelta = inputContext.softwareInputPanelRect.height
                }
            }
        }

        SoftwareInputPanel {
            id: softwareInputPanel
            active: inputContext.customSoftwareInputPanelVisible
            anchors.bottom: parent.bottom

            onHeightChanged: {
                windowContent.heightDelta = height
            }

            Loader {
                id: softwareInputPanelLoader
                width: parent.width
                sourceComponent: inputContext.customSoftwareInputPanelComponent
            }
        }

        Snapshot {
            id: snapshot
            anchors.top: parent.top
            anchors.left: parent.left
            width: screen.displayWidth
            height: screen.displayHeight
            snapshotWidth: screen.displayWidth
            snapshotHeight: screen.displayHeight
            opacity: 0
            transform: Rotation { id: snapshotRotation;
                                  origin.x: screen.displayHeight / 2; origin.y: screen.displayHeight / 2;
                                  angle: 0 }
        }

        state: screen.orientationString

        states: [
            State {
                name: "Landscape"
                PropertyChanges { target: window; /* rotation: 0;*/ portrait: false; }
                PropertyChanges { target: windowRotation; angle: 0 }
                PropertyChanges { target: windowTranslate; x: 0; y: 0 }
                PropertyChanges { target: snapshot; anchors.leftMargin: 0; anchors.topMargin: 0 }
            },
            State {
                name: "Portrait"
                PropertyChanges { target: window; /* rotation: 270;*/ portrait: true; }
                PropertyChanges { target: windowRotation; angle: 270 }
                PropertyChanges { target: windowTranslate; x: 0; y: 0 }
                PropertyChanges { target: snapshot; anchors.leftMargin: 0; anchors.topMargin: 0 }
            },
            State {
                name: "LandscapeInverted"
                PropertyChanges { target: window; /* rotation: 180;*/ portrait: false; }
                PropertyChanges { target: windowRotation; angle: 180 }
                PropertyChanges { target: windowTranslate; x: -374; y: 0 }
                PropertyChanges { target: snapshot; anchors.leftMargin: 374; anchors.topMargin: 0 }
            },
            State {
                name: "PortraitInverted"
                PropertyChanges { target: window; /* rotation: 90;*/ portrait: true; }
                PropertyChanges { target: windowRotation; angle: 90 }
                PropertyChanges { target: windowTranslate; x: 0; y: -374 }
                PropertyChanges { target: snapshot; anchors.leftMargin: 0; anchors.topMargin: 374 }
            }
        ]

        transitions: [
        Transition {
            // use this transition when sip is visible
            from: (inputContext.softwareInputPanelVisible ?  "*" : "disabled")
            to:   (inputContext.softwareInputPanelVisible ?  "*" : "disabled")
            PropertyAction { target: windowRotation; properties: "angle"; }
            ScriptAction {
                script: {
                    root.orientationChangeAboutToStart();
                    platformWindow.startSipOrientationChange(windowRotation.angle);
                    // note : we should really connect these signals to MInputMethodState
                    // signals so that they are emitted at the appropriate time
                    // but there aren't any
                    root.orientationChangeStarted();
                    root.orientationChangeFinished();
                }
            }
        },
        Transition {
            // use this transition when sip is not visible
            from: (screen.minimized ? "disabled" : (inputContext.softwareInputPanelVisible ? "disabled" : "*"))
            to:   (screen.minimized ? "disabled" : (inputContext.softwareInputPanelVisible ? "disabled" : "*"))
            SequentialAnimation {
                alwaysRunToEnd: true

                ScriptAction {
                    script: {
                        snapshot.take();
                        snapshot.opacity = 1.0;
                        snapshotRotation.angle = -windowRotation.angle;
                        snapshot.smooth = false; // Quick & coarse rotation consistent with MTF
                        platformWindow.animating = true;
                        root.orientationChangeAboutToStart();
                    }
                }
                PropertyAction { target: window; properties: "portrait"; }
                ScriptAction {
                    script: {
                        windowContent.opacity = 0.0;
                        root.orientationChangeStarted();
                    }
                }
                ParallelAnimation {
                    NumberAnimation { target: windowContent; property: "opacity";
                                      to: 1.0; easing.type: Easing.InOutExpo; duration: 600; }
                    NumberAnimation { target: snapshot; property: "opacity";
                                      to: 0.0; easing.type: Easing.InOutExpo; duration: 600; }
                    RotationAnimation { target: windowRotation; property: "angle";
                                        direction: RotationAnimation.Shortest;
                                        easing.type: Easing.InOutExpo; duration: 600; }
                }
                ScriptAction {
                    script: {
                        snapshot.free();
                        root.orientationChangeFinished();
                        platformWindow.animating = false;
                    }
                }
            }
        }
        ]

        focus: true
        Keys.onReleased: {
            if (event.key == Qt.Key_I && event.modifiers == Qt.AltModifier) {
                theme.inverted = !theme.inverted;
            }
            if (event.key == Qt.Key_E && event.modifiers == Qt.AltModifier) {
                if(screen.currentOrientation == Screen.Landscape) {
                    screen.allowedOrientations = Screen.Portrait;
                } else if(screen.currentOrientation == Screen.Portrait) {
                    screen.allowedOrientations = Screen.LandscapeInverted;
                } else if(screen.currentOrientation == Screen.LandscapeInverted) {
                    screen.allowedOrientations = Screen.PortraitInverted;
                } else if(screen.currentOrientation == Screen.PortraitInverted) {
                    screen.allowedOrientations = Screen.Landscape;
                }
            }
        }
    }
}
