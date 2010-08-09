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

Item {
    id: basicSlider;

    property variant sliderEdgeOffset: 6
    property alias value: model.value
    property alias inverted: model.inverted
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    property alias tooltipText: tooltip.text

    height: 22
    width: 108


    BorderImage {
        id: sliderBase
        width: parent.width - 12
        x: 6
        anchors.verticalCenter: parent.verticalCenter
        source: Qt.resolvedUrl("images/slider-background.png");

        border.left: 10
        border.top: 0
        border.right: 10
        border.bottom: 0


        MouseArea {
            id: grooveArea
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            height: 20
            onPressed: { model.setPosition(mouseX - knob.width/2); }
            hoverEnabled: tooltip.text;
        }

        BorderImage {
            id: knob
            x: model.position
            anchors.verticalCenter: parent.verticalCenter
            width: 22
            height: 16

            border.left: 10
            border.top: 10
            border.right: 10
            border.bottom: 10

            source: Qt.resolvedUrl("images/slider-handle.png");

            MouseArea {
                id: knobArea
                anchors.fill: knob
                hoverEnabled: tooltip.text
                drag.target: knob
                drag.axis: "XAxis"
                drag.minimumX: -sliderEdgeOffset
                drag.maximumX: sliderBase.width - knob.width / 2 - sliderEdgeOffset
            }

            states: [
                State {
                    name: "hover"
                    when: !knobArea.pressed && knobArea.containsMouse
                    PropertyChanges { target: knob; source: Qt.resolvedUrl("images/slider-handle-hover.png"); }
                },
                State {
                    name: "pressed"
                    when: knobArea.pressed
                    PropertyChanges { target: knob; source: Qt.resolvedUrl("images/slider-handle-active.png"); }
                }
            ]
        }
    }

    RangeModel {
        id: model
        minimumValue: 0
        maximumValue: 100
        positionAtMinimum: -sliderEdgeOffset
        positionAtMaximum: sliderBase.width - knob.width / 2 - sliderEdgeOffset
        position: knob.x
    }

    TooltipLoader {
        id: tooltip;
        anchors.fill: parent;

        property bool pressDismiss: false;
        property bool containsMouse: grooveArea.containsMouse
        || knobArea.containsMouse
        property bool mousePressed: grooveArea.pressed
        || knobArea.pressed;
        property bool resetState: !containsMouse && !mousePressed

        // Dismiss on press
        onMousePressedChanged: {
            if (mousePressed)
                pressDismiss = true;
        }

        // Reset on leave and release
        // ### Ugly: We use this timer to debounce the value of this property when the
        //     mouse moves from one area to another
        onResetStateChanged: {
            if (resetState)
                dismissTimer.start();
        }
        Timer {
            id: dismissTimer;
            interval: 10;
            onTriggered: {
                // Condition for "onExited" event
                if (tooltip.resetState)
                    tooltip.pressDismiss = false;
            }
        }

        shown: containsMouse && !pressDismiss;
    }
}
