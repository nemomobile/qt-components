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

    property alias enabled: thumb.enabled

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleType: Style.Switch
    }

    Background {
        anchors.fill: parent
        style: meegostyle
    }

    MaskedImage {
        anchors.fill: parent
        style: meegostyle
        maskProperty: "sliderMask"
        imageProperty: "sliderImage"
        imageXOffset: (thumb.x + thumb.width / 2) - width
    }

    ScalableImage {
        id: thumb
        anchors.verticalCenter: parent.verticalCenter
        property bool enabled: false

        style: meegostyle
        imageProperty: "thumbImage"

        states: [
            State {
                name: "off"
                when: !mousearea.drag.active && !enabled
                PropertyChanges {
                    restoreEntryValues: false
                    target: thumb
                    x: mousearea.drag.minimumX
                }
            },
            State {
                name: "on"
                when: !mousearea.drag.active && enabled
                PropertyChanges {
                    restoreEntryValues: false
                    target: thumb
                    x: mousearea.drag.maximumX
                }
            },
            State {
                name: "dragging"
                when: mousearea.drag.active
            }]

        transitions: [
            // XXX Waiting for QTBUG-12805
            // After that is fixed, we can use a transition "*" to "*"
            Transition {
                from: "*"
                to: "off"
                SmoothedAnimation { properties: "x"; velocity: 500; maximumEasingTime: 0 }
            },
            Transition {
                from: "*"
                to: "on"
                SmoothedAnimation { properties: "x"; velocity: 500; maximumEasingTime: 0 }
            }
        ]
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent

        drag {
            axis: Drag.XAxis
            minimumX: 0
            maximumX: parent.width - thumb.width
            target: thumb
            onActiveChanged: {
                if (!mousearea.drag.active) {
                    thumb.enabled = (thumb.x > mousearea.drag.maximumX / 2)
                }
            }
        }
    }
}
