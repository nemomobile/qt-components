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
import com.nokia.symbian.themebridge 1.0
import "AppManager.js" as AppManager

Rectangle {
    id: fader

    property int animationDuration: 250

    signal clicked()

    function isActive() {
        return internal.state == "Active";
    }

    function activate() {
        if (!fader.isActive()) {
            internal.state = "Active";
            fader.enabled = true;
        }
    }

    function deactivate() {
        if (fader.isActive()) {
            fader.enabled = false;
            internal.state = "Inactive";
        }
    }

    width: style.current.get("appRectWidth"); height: style.current.get("appRectHeight")
    color: "black"

    Component.onCompleted: fader.parent = AppManager.rootObject()

    Connections {
        target: screen
        onOrientationChanged: {
            fader.width = style.current.get("appRectWidth");
            fader.height = style.current.get("appRectHeight");
        }
    }

    Style {
        id: style
        styleClass: "Fader"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: fader.clicked()
    }

    StateGroup {
        id: internal

        state: "Inactive"

        states: [
            State {
                name: "Inactive"
                PropertyChanges { target: fader; opacity: 0 }
            },
            State {
                name: "Active"
                PropertyChanges { target: fader; opacity: 0.5 }
            }
        ]

        transitions: Transition {
            PropertyAnimation { duration: fader.animationDuration; properties: "opacity"; easing.type: Easing.InOutCubic }
        }
    }
}
