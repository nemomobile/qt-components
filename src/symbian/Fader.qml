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

import Qt 4.7

Item {
    id: root

    property real dimm: 0.8
    property int animationDuration: 500
    property Item visualParent: null

    signal clicked

    anchors.fill: parent
    state: "Hidden"

    Rectangle {
        id: fader

        anchors.fill: parent
        opacity: 0.0
        color: "black"

        property QtObject originalParent: parent

        function findRoot() {
            var next = parent

            if (next != null) {
                while (next.parent)
                    next = next.parent
            }
            return next
        }

        //eat mouse events
        MouseArea {
            id: mouseEventEater
            anchors.fill: parent
            enabled: fader.opacity != 0.0
            onClicked: root.clicked()
        }
    }

    states: [
        State {
            name: "Visible"
            PropertyChanges { target: fader; opacity: dimm }
        },
        State {
            name: "Hidden"
            PropertyChanges { target: fader; opacity: 0.0 }
        }
    ]

    transitions: [
        Transition {
            from: "Hidden"; to: "Visible"
            //reparent fader whenever it is going to be visible
            SequentialAnimation {
                ScriptAction {script: {
                        // the algorithm works in the following way:
                        // First:  Check if visualParent property is set; if yes, center the fader in visualParent
                        // Second: If not, center inside window content element
                        // Third:  If no window was found, use root window
                        fader.originalParent = root.parent
                        if (visualParent != null) {
                            root.parent = visualParent
                        } else if (typeof window != "undefined") {
                            root.parent = window
                        } else {
                            var appRoot = fader.findRoot()
                            if (appRoot != null)
                                root.parent = appRoot
                        }
                    }
                }
                NumberAnimation {properties: "opacity"; duration: animationDuration}
            }
        },
        Transition {
            from: "Visible"; to: "Hidden"
            SequentialAnimation {
                NumberAnimation { properties: "opacity"; duration: animationDuration }
                ScriptAction { script: root.parent = fader.originalParent }
            }
        }
    ]
}
