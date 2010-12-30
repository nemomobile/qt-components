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
import Qt.labs.components.native 1.0
import com.meego.themebridge 1.0

Item {
    id: window
    width: screen.width
    height: screen.height

    Snapshot {
        id: snapshot
        anchors.centerIn: parent
        width: screen.width
        height: screen.height
        snapshotWidth: screen.width
        snapshotHeight: screen.height
        z: 100
        opacity: 0
    }

    // Read only property true if window is in landscape
    property bool inLandscape: true
    // Read only property true if window is in portrait
    property bool inPortrait: false

    signal orientationChangeAboutToStart
    signal orientationChangeStarted
    signal orientationChangeFinished

    state: screen.orientationString

    states:  [
        State {
            name: "Landscape"
            PropertyChanges {
                target: window
                rotation: 0
                width: screen.width
                height: screen.height
                x: 0
                y: 0
                inLandscape: true
                inPortrait: false
            }
        },
        State {
            name: "LandscapeInverted"
            PropertyChanges {
                target: window
                rotation: 180
                width: screen.width
                height: screen.height
                x: 0
                y: 0
                inLandscape: true
                inPortrait: false
            }
        },
        State {
            name: "Portrait"
            PropertyChanges {
                target: window
                rotation: 270
                width: screen.height
                height: screen.width
                x: (screen.width - screen.height) / 2
                y: -(screen.width - screen.height) / 2
                inLandscape: false
                inPortrait: true
            }
        },
        State {
            name: "PortraitInverted"
            PropertyChanges {
                target: window
                rotation: 90
                width: screen.height
                height: screen.width
                x: (screen.width - screen.height) / 2
                y: -(screen.width - screen.height) / 2
                inLandscape: false
                inPortrait: true
            }
        }
    ]

    property int duration: 800
    transitions: Transition {
        SequentialAnimation {
            ScriptAction {
                script: {
                    window.orientationChangeAboutToStart()
                    snapshot.take()
                    snapshot.opacity = 1
                    snapshot.rotation = -window.rotation
                    window.opacity = 0

                    window.orientationChangeStarted()
                }
            }
            PropertyAction { target: window; properties: "x,y,width,height" }
            ParallelAnimation {
                NumberAnimation { target: window; property: "opacity"; to: 1; easing.type: Easing.InOutExpo; duration: window.duration }
                NumberAnimation { target: snapshot; property: "opacity"; to: 0; easing.type: Easing.InOutExpo; duration: window.duration }
                RotationAnimation { target: window; property: "rotation"; direction: RotationAnimation.Shortest; easing.type: Easing.InOutExpo; duration: window.duration }
            }
            ScriptAction {
                script: {
                    snapshot.free();
                    window.orientationChangeFinished();
                }
            }
        }
    }

    focus: true
    Keys.onReleased: {
        if(event.key == Qt.Key_E && event.modifiers == Qt.AltModifier &&
                !screen.orientationLocked) {
            if(screen.orientation == Screen.Landscape) {
                screen.orientation = Screen.Portrait;
            } else if(screen.orientation == Screen.Portrait) {
                screen.orientation = Screen.LandscapeInverted;
            } else if(screen.orientation == Screen.LandscapeInverted) {
                screen.orientation = Screen.PortraitInverted;
            } else if(screen.orientation == Screen.PortraitInverted) {
                screen.orientation = Screen.Landscape;
            }
        }
    }
}
