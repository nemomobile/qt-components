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
import com.nokia.symbian 1.0
import com.nokia.symbian.themebridge 1.0

ImplicitSizeItem {
    id: progressBar

    // Common Public API
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property real value: 0.0
    property bool indeterminate: false

    implicitWidth: style.current.preferredWidth
    implicitHeight: style.current.preferredHeight

    Style {
        id: style
        styleClass: "ProgressBar"
    }

    Frame {
        id: background
        frameName: style.current.get("backgroundFrame")
        frameType: style.current.get("backgroundType")

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        width: parent.width
        height: parent.height

        states: [
            State {
                name: "determinate"
                when: indeterminate == false
                PropertyChanges {
                    target: frame
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    visible: false
                }
                PropertyChanges {
                    target: propertyAnimation
                    running: false
                }
            },
            State {
                name: "indeterminate"
                when: indeterminate == true
                PropertyChanges {
                    target: frame
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    visible: true
                }
                PropertyChanges {
                    target: propertyAnimation
                    running: true
                }
            }
        ]

        PropertyAnimation { id: propertyAnimation; loops: Animation.Infinite; running: true; target: indeterminateAnimation; property: "animationOffset"; from: 0; to: 200; easing.type: Easing.Linear; duration: 5000 }

        Item {
            clip: true

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: progressWidth()

            function progressWidth() {
                if (value <= minimumValue)
                    return 0;
                else if (value >= maximumValue)
                    return progressBar.width;
                else
                    return ((value - minimumValue) / (maximumValue - minimumValue)) * progressBar.width;
            }

            Frame {
                id: frame
                frameName: style.current.get("contentsFrame")
                frameType: style.current.get("contentsType")

                anchors.left: parent.left
                anchors.top: parent.top

                width: progressBar.width
                height: parent.height
            }
        }

        ProgressBarAnimation {
            id: indeterminateAnimation
            anchors.fill: parent

            animationIcon: style.current.get("indeterminateIcon")
            animationMask: style.current.get("indeterminateMask")
        }
    }
}

