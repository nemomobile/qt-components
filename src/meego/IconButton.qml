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
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: root

    property alias iconId: icon.iconId
    property alias styleObjectName: meegostyle.styleObjectName
    // the icon button needs to export its mode in case it has a background
    property alias mode: meegostyle.mode
    signal clicked


    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MButtonIconStyle"
        mode: area.containsMouse && area.pressed ? "pressed" : "default"
    }

    MouseArea {
        id: area
        anchors.fill: parent
        onClicked: root.clicked()
    }

    IconGlow {
        id: glow
        anchors.fill: icon
        opacity: 0

        iconId: icon.iconId
        glowColor: meegostyle.current.get("glowColor")
        glowRadius: meegostyle.current.get("glowRadius")
    }

    Icon {
        id: icon
        anchors.centerIn: parent
        width: meegostyle.current.get("iconSize").width
        height: meegostyle.current.get("iconSize").height

        states: State {
            name: "pressed"
            when: area.containsMouse && area.pressed
            PropertyChanges {
                target: icon
                scale: 1 - meegostyle.current.get("shrinkFactor")
            }
            PropertyChanges {
                target: glow
                opacity: 0
            }
        }

        transitions: [
            Transition {
                to: "pressed"
                PropertyAnimation {
                    target: icon
                    property: "scale"
                    easing.type: Easing.InCubic
                    duration: meegostyle.current.get("shrinkDuration")
                }
            },
            Transition {
                to: ""
                SequentialAnimation {
                    PropertyAnimation {
                        target: icon
                        property: "scale"
                        easing.type: Easing.OutCubic
                        duration: meegostyle.current.get("shrinkDuration")
                    }
                    PropertyAnimation {
                        target: glow
                        properties: "opacity"
                        from: 0
                        to: 1
                        easing.type: Easing.OutSine
                        duration: meegostyle.current.get("glowDuration") / 2
                    }
                    PropertyAnimation {
                        target: glow
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: meegostyle.current.get("glowDuration") / 2
                        easing.type: Easing.OutSine
                    }
                }
            }
        ]
    }
}
