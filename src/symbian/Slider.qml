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
import Qt.labs.components 1.0 as QtComponents

ImplicitSizeItem {
    id: slider

    // Common Public API
    property alias stepSize: model.stepSize
    property alias minimumValue: model.minimumValue
    property alias maximumValue: model.maximumValue
    property alias value: model.value
    property int orientation: Qt.Horizontal
    property bool pressed: handleMouseArea.pressed || track.keysActive
    property bool updateValueWhileDragging: true

    // Symbian specific
    // TODO: property bool toolTipVisible: true
    property alias inverted: model.inverted

    signal valueChanged(real value)
    implicitWidth: style.current.preferredWidth
    implicitHeight: style.current.preferredHeight

    QtComponents.RangeModel {
        id: model
        value: 0.0
        stepSize: 1.0
        minimumValue: 0.0
        maximumValue: 100.0
        positionAtMinimum: 0
        positionAtMaximum: orientation == Qt.Horizontal ? track.width - handle.width : track.height - handle.height
        onValueChanged: slider.valueChanged(value)
    }

    Style {
        id: style
        styleClass: "Slider"
        mode: orientation == Qt.Horizontal ? "horizontal" : "vertical"
    }

    Frame {
        id: track
        objectName: "track"
        // TODO: property ToolTip toolTip
        property bool tapOnTrack: false
        property bool keysActive: false

        height: orientation == Qt.Horizontal ? style.current.get("trackHeight") : undefined
        width: orientation == Qt.Horizontal ? undefined : style.current.get("trackHeight")
        states: [
            State {
                name: "Pressed"
                when: handleMouseArea.pressed
                PropertyChanges { target: track; frameName: style.current.get("trackPressed"); }
            }
        ]

        frameName: style.current.get("trackFrame")
        frameType: style.current.get("trackType")

        anchors.left: orientation == Qt.Horizontal ? slider.left : undefined
        anchors.right: orientation == Qt.Horizontal ? slider.right : undefined
        anchors.top: orientation == Qt.Horizontal ? undefined : slider.top
        anchors.bottom: orientation == Qt.Horizontal ? undefined : slider.bottom
        anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : slider.horizontalCenter
        anchors.verticalCenter: orientation == Qt.Horizontal ? slider.verticalCenter : undefined

        anchors.leftMargin: style.current.get("marginLeft")
        anchors.rightMargin: style.current.get("marginRight")
        anchors.topMargin: style.current.get("marginTop")
        anchors.bottomMargin: style.current.get("marginBottom")

        MouseArea {
            id: trackMouseArea
            objectName: "trackMouseArea"

            anchors.fill: parent

            onClicked: {
                if (track.tapOnTrack) {
                    if (orientation == Qt.Horizontal) {
                        if (handle.x > (mouseX - handle.width / 2)) {
                            model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                            handle.x = model.position
                        } else {
                            model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                            handle.x = model.position
                        }
                    } else {
                        if (handle.y > (mouseY - handle.height / 2)) {
                            model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                            handle.y = model.position
                        } else {
                            model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                            handle.y = model.position
                        }
                    }
                }
            }

            Icon {
                id: handle
                objectName: "handle"
                x: orientation == Qt.Horizontal ? model.position : 0
                y: orientation == Qt.Horizontal ? 0 : model.position

                height: style.current.get("handleHeight")
                width: style.current.get("handleWidth")

                anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                states: [
                    State {
                        name: "Pressed"
                        when: handleMouseArea.pressed
                        PropertyChanges { target: handle; iconName: style.current.get("handlePressed"); }
                    }
                ]

                iconName: style.current.get("handleIcon")

                MouseArea {
                    id: handleMouseArea
                    objectName: "handleMouseArea"

                    height: style.current.get("handleTouchAreaHeight")
                    width: style.current.get("handleTouchAreaWidth")
                    anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                    anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                    drag.target: handle
                    drag.axis: orientation == Qt.Horizontal ? Drag.XAxis : Drag.YAxis
                    drag.minimumX: orientation == Qt.Horizontal ? model.positionAtMinimum : 0
                    drag.maximumX: orientation == Qt.Horizontal ? model.positionAtMaximum : 0
                    drag.minimumY: orientation == Qt.Horizontal ? 0 : model.positionAtMinimum
                    drag.maximumY: orientation == Qt.Horizontal ? 0 : model.positionAtMaximum

                    onPositionChanged: {
                        if (updateValueWhileDragging) {
                            model.position = orientation == Qt.Horizontal ? handle.x : handle.y
                            // TODO: updateToolTipPos()
                        }
                    }
                    onReleased: {
                        if (!updateValueWhileDragging)
                            model.position = orientation == Qt.Horizontal ? handle.x : handle.y
                    }
                }
            }

            Timer {
                id: keyActivity
                interval: 750
                onTriggered: track.keysActive = false
            }

            /* TODO: ToolTip missing
            states:  [
                State {
                    when: slider.pressed
                    name: "Visible"
                    StateChangeScript {
                        script: {
                            keyActivity.running = false
                            if (!track.toolTip)
                                track.toolTip = window.showToolTip(toolTip)
                                updateToolTipPos()
                        }
                    }
                },
                State {
                    when: !slider.pressed
                    name: "Invisible"
                    StateChangeScript {
                        script: hideToolTip()
                    }
                }
            ] */
        }
    }

    Keys.onPressed: {
        handleKeyEvent(event)
        track.keysActive = true
        keyActivity.restart()
        // TODO: if (track.toolTip) updateToolTipPos()
    }

    /* TODO: ToolTip missing
    function hideToolTip() {
        if (track.toolTip && toolTipVisible) {
            track.toolTip.dismiss();
            track.toolTip = null;
        }
    }

    function updateToolTipPos() {
        var point = null;
        if (track.toolTip) {
            if (orientation == Qt.Horizontal) {
                point = window.mapFromItem(track, handle.x, 0);
                track.toolTip.x = point.x;
                track.toolTip.y = point.y - style.current.get("toolTipDistance") - track.toolTip.height;
            } else {
                point = window.mapFromItem(track, 0, handle.y);
                track.toolTip.x = point.x - style.current.get("toolTipDistance") - track.toolTip.width;
                track.toolTip.y = point.y;
            }
        }
    }

    Component {
        id: toolTip
        ToolTip {
            text:  model.value
        }
    } */

    function handleKeyEvent(keyEvent) {
        if (orientation == Qt.Horizontal) {
            if (keyEvent.key == Qt.Key_Left) {
                model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize;
                handle.x = model.position;
                keyEvent.accepted = true;
            } else if (keyEvent.key == Qt.Key_Right) {
                model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize;
                handle.x = model.position;
                keyEvent.accepted = true;
            }
        } else { //Vertical
            if (keyEvent.key == Qt.Key_Up) {
                model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize;
                handle.x = model.position;
                keyEvent.accepted = true;
            } else if (keyEvent.key == Qt.Key_Down) {
                model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize;
                handle.x = model.position;
                keyEvent.accepted = true;
            }
        }
    }
}
