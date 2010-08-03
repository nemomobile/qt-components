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

    property double clientY: statusbar.y + statusbar.height + titlebar.height;
    property bool statusbarVisible: true

    width: screen.width;
    height: screen.height;
    property alias orientation: screen.orientation;

    Snapshot {
        id: snapshot;
        anchors.centerIn: parent;
    }

    Screen {
        id : screen;


        onOrientationChanged: {
            var delta = window.rotation - rotation;
            // normalize to -90...180
            if (delta <= -180) {
                window.rotation += 360;
                delta += 360;
            } else if (delta > 180) {
                window.rotation -= 360;
                delta -= 360;
            }

            if (Math.abs(delta) < 100) {
                // no need to take a snapshot for 180 degrees
                snapshot.take();
                snapshot.opacity = 1;
                snapshot.rotation = -window.rotation;
                window.opacity = 0;
            }

            if (orientation == Screen.Landscape || orientation == Screen.LandscapeInverted) {
                window.width = width;
                window.height= height;
                window.x = 0;
                window.y = 0;
            } else {
                window.width = height;
                window.height= width;
                window.x = (width - height)/2;
                window.y = -(width - height)/2;
            }

            if (orientation == Screen.Portrait)
                state = "Portrait";
            else if (orientation == Screen.Landscape)
                state = "Landscape";
            if (orientation == Screen.PortraitInverted)
                state = "Portrait";
            else if (orientation == Screen.LandscapeInverted)
                state = "LandscapeInverted";
        }
        states:  [
            State {
                name: "Landscape"
                PropertyChanges { target: window; rotation : 0 }
            },
            State {
                name: "LandscapeInverted"
                PropertyChanges { target: window; rotation : 180 }
            },
            State {
                name: "Portrait"
                PropertyChanges { target: window; rotation : 270 }
            },
            State {
                name: "PortraitInverted"
                PropertyChanges { target: window; rotation : 90 }
            }
        ]

        transitions: Transition {
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: window; property: "opacity"; to: 1; duration: 300 }
                    NumberAnimation { target: snapshot; property: "opacity"; to: 0; duration: 300 }
                    PropertyAnimation { target: window; properties: "rotation"; easing.type: Easing.InOutQuad; duration: 300 }
                }
                ScriptAction { script: snapshot.free(); }
            }
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
