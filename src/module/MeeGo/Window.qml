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
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Rectangle {
    id: window

    property int orientation: Screen.Portrait;

    property bool statusbarVisible: true

    property double __pageX: 0;
    property double __pageY: statusbar.y + statusbar.height + titlebar.height;
    property alias __pageWidth: window.width;
    property double __pageHeight: height - __pageY;

    Component.onCompleted: {
        orientation = screen.orientation;
    }

    Snapshot {
        id: snapshot;
        anchors.centerIn: parent;
    }

    function setOrientation() {
        var delta = window.rotation - screen.rotation;
        // normalize to -90...180
        if (delta <= -180) {
            delta += 360;
        } else if (delta > 180) {
            delta -= 360;
        }

        if (Math.abs(delta) < 100) {
            // no need to take a snapshot for 180 degrees
            snapshot.take();
            snapshot.opacity = 1;
            snapshot.rotation = -window.rotation;
            window.opacity = 0;
        }

        window.orientation = screen.orientation;
    }

    Connections {
        target: screen;
        onOrientationChanged: { window.setOrientation() }
    }
    states:  [
        State {
            name: "Landscape"
            when: (orientation == Screen.Landscape)
            PropertyChanges {
                target: window;
                rotation : 0
                width: screen.width;
                height: screen.height;
                x: 0;
                y: 0;
            }
        },
        State {
            name: "LandscapeInverted"
            when: (orientation == Screen.LandscapeInverted)
            PropertyChanges {
                target: window;
                rotation : 180
                width: screen.width;
                height: screen.height;
                x: 0;
                y: 0;
            }
        },
        State {
            name: "Portrait"
            when: (orientation == Screen.Portrait)
            PropertyChanges {
                target: window;
                rotation : 270
                width: screen.height;
                height: screen.width;
                x: (screen.width - screen.height)/2;
                y: -(screen.width - screen.height)/2;
            }
        },
        State {
            name: "PortraitInverted"
            when: (orientation == Screen.PortraitInverted)
            PropertyChanges {
                target: window;
                rotation : 90
                width: screen.height;
                height: screen.width;
                x: (screen.width - screen.height)/2;
                y: -(screen.width - screen.height)/2;
            }
        }
    ]

    transitions: Transition {
        SequentialAnimation {
            PropertyAction { target: window; properties: "x,y,width,height" }
            ParallelAnimation {
                NumberAnimation { target: window; property: "opacity"; to: 1; duration: 300 }
                NumberAnimation { target: snapshot; property: "opacity"; to: 0; duration: 300 }
                RotationAnimation { target: window; properties: "rotation"; easing.type: Easing.InOutQuad; duration: 300 }
            }
            ScriptAction { script: snapshot.free(); }
        }
    }

    StatusBar {
        id: statusbar;

        width: parent.width;
        orientation: screen.orientation;

        states: State {
                name: 'hidden';
                when:  (statusbar.parent.statusbarVisible == false);
                PropertyChanges { target: statusbar; y: -statusbar.height; }
                PropertyChanges { target: statusbar; opacity: 0; }
        }

        transitions: Transition {
            SequentialAnimation {
                NumberAnimation { target: statusbar; properties: "y,opacity"; duration: 300; easing.type: Easing.InOutQuad }
            }
        }
    }
    TitleBar {
        id: titlebar;
        anchors.top: statusbar.bottom;
        onMinimize: { screen.minimized = true; }
        onQuit:  { Qt.quit(); }
    }
}
