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

    implicitWidth: escapeStyle.preferredWidth
    implicitHeight: escapeStyle.preferredHeight

    property bool back: false
    signal closeClicked
    signal backClicked

    Style {
        id: escapeStyle
        styleClass: "MEscapeButtonPanelStyle"
        mode: back.state == "backButtonShown" ? back.mode : close.mode
    }

    // XXX Use MWarpAnimation style to get proper animation parameters
    // These values are true for the development theme
    property int warpDistance: 30
    property int warpDuration: 300
    property int warpInDelay: 200
    property int warpInCurve: Easing.OutCubic
    property int warpOutCurve: Easing.InCubic

    Background {
        anchors.fill: parent
        style: escapeStyle
    }

    IconButton {
        id: close
        anchors.centerIn: parent
        onClicked: root.closeClicked()

        iconId: escapeStyle.current.get("closeButtonIconId")
        styleObjectName: escapeStyle.current.get("closeButtonObjectName")
    }

    IconButton {
        id: back
        anchors.centerIn: parent
        onClicked: root.backClicked()

        iconId: escapeStyle.current.get("backButtonIconId")
        styleObjectName: escapeStyle.current.get("backButtonObjectName")

        // Hidden state
        anchors.horizontalCenterOffset: root.warpDistance
        opacity: 0

        states: State {
            name: "backButtonShown"
            when: root.back
            PropertyChanges {
                target: back
                anchors.horizontalCenterOffset: 0
                opacity: 1
            }
            PropertyChanges {
                target: close
                anchors.horizontalCenterOffset: -root.warpDistance
                opacity: 0
            }
        }

        transitions: [
            Transition {
                from: ""
                to: "backButtonShown"
                ParallelAnimation {
                    SequentialAnimation {
                        PauseAnimation {
                            duration: root.warpInDelay
                        }
                        PropertyAnimation {
                            target: back
                            properties: "horizontalCenterOffset, opacity"
                            duration: root.warpDuration
                            easing.type: root.warpInCurve
                        }
                    }
                    PropertyAnimation {
                        target: close
                        properties: "horizontalCenterOffset, opacity"
                        duration: root.warpDuration
                        easing.type: root.warpOutCurve
                    }
                }
            },
            Transition {
                from: "backButtonShown"
                to: ""
                ParallelAnimation {
                    SequentialAnimation {
                        PauseAnimation {
                            duration: root.warpInDelay
                        }
                        PropertyAnimation {
                            target: close
                            properties: "horizontalCenterOffset, opacity"
                            duration: root.warpDuration
                            easing.type: root.warpInCurve
                        }
                    }
                    PropertyAnimation {
                        target: back
                        properties: "horizontalCenterOffset, opacity"
                        duration: root.warpDuration
                        easing.type: root.warpOutCurve
                    }
                }
            }
        ]
    }
}
