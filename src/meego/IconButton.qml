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
import "UIConstants.js" as UI

ImplicitSizeItem {
    id: root

    property alias pressed: area.buttonPressed
    property alias text: label.text
    property alias iconSource: icon.source
    property alias iconWidth: icon.width
    property alias iconHeight: icon.height

    signal clicked

    implicitWidth: Math.max(88, icon.width)
    implicitHeight: Math.max(60, icon.height)

    opacity: enabled ? UI.OPACITY_ENABLED : UI.OPACITY_DISABLED

    MouseArea {
        id: area
        anchors.fill: parent
        onClicked: root.clicked()
        property bool buttonPressed: containsMouse && pressed
    }

    IconGlow {
        id: glow
        anchors.fill: icon
        opacity: 0

        source: icon.source
        sourceSize: icon.sourceSize
        glowColor: "white"
        glowRadius: 8
    }

    Image {
        id: icon
        anchors.centerIn: parent
        smooth: true

        transform: Scale {
            id: scaleTransform
            origin.x: icon.width / 2
            origin.y: icon.height / 2
            xScale: label.scale
            yScale: label.scale
        }

        states: State {
            name: "pressed"
            when: area.containsMouse && area.pressed
            PropertyChanges {
                target: label
                scale: 0.8
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
                    property: "scale"
                    easing.type: Easing.InCubic
                    duration: 100
                }
            },
            Transition {
                to: ""
                SequentialAnimation {
                    PropertyAnimation {
                        property: "scale"
                        easing.type: Easing.OutCubic
                        duration: 100
                    }
                    PropertyAnimation {
                        target: glow
                        properties: "opacity"
                        from: 0
                        to: 1
                        easing.type: Easing.OutSine
                        duration: 400
                    }
                    PropertyAnimation {
                        target: glow
                        property: "opacity"
                        from: 1
                        to: 0
                        easing.type: Easing.OutSine
                        duration: 400
                    }
                }
            }
        ]
    }

    Label {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: icon.bottom
        anchors.topMargin: -2

        states: State {
            name: "buttonHasLabel"
            when: label.text

            PropertyChanges {
                target: root
                implicitWidth: {
                    var contentsWidth = Math.max(icon.width, label.width);
                    return Math.max(88, contentsWidth);
                }
                implicitHeight: {
                    var contentsHeight = icon.height + label.height + label.anchors.topMargin;
                    return Math.max(60, contentsHeight);
                }
            }

            PropertyChanges {
                target: icon
                anchors.verticalCenterOffset: -(label.height + label.anchors.topMargin) / 2
            }

            PropertyChanges {
                target: scaleTransform
                origin.y: {
                    var contentsHeight = icon.height + label.height + label.anchors.topMargin;
                    return contentsHeight / 2;
                }
            }
        }
    }
}
