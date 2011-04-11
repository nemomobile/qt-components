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
import "." 1.0
import Qt.labs.components 1.0 as QtComponents

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias stepSize: model.stepSize
    property alias minimumValue: model.minimumValue
    property alias maximumValue: model.maximumValue
    property alias value: model.value
    property int orientation: Qt.Horizontal
    property bool pressed: handleMouseArea.pressed || track.keysActive
    property bool updateValueWhileDragging: true // deprecated
    onUpdateValueWhileDraggingChanged: console.log("warning: Slider.updateValueWhileDragging is deprecated.")
    property bool valueIndicatorVisible: false
    property string valueIndicatorText: ""

    // Symbian specific
    property bool toolTipVisible: false // deprecated
    property alias inverted: model.inverted

    onToolTipVisibleChanged: {
        valueIndicatorVisible = toolTipVisible
        console.log("warning: Slider.toolTipVisible is deprecated. Use Slider.valueIndicatorVisible instead")
    }

    signal valueChanged(real value)
    implicitWidth: orientation == Qt.Horizontal ? 150 : platformStyle.graphicSizeSmall
    implicitHeight: orientation == Qt.Horizontal ? platformStyle.graphicSizeSmall : 150

    QtComponents.RangeModel {
        id: model
        value: 0.0
        stepSize: 0.0
        minimumValue: 0.0
        maximumValue: 1.0
        positionAtMinimum: 0
        positionAtMaximum: orientation == Qt.Horizontal ? track.width - handle.width : track.height - handle.height
        onValueChanged: root.valueChanged(value)
    }

    BorderImage {
        id: track
        objectName: "track"
        property bool tapOnTrack: false
        property bool keysActive: false

        states: [
            State {
                name: "Horizontal"
                when: orientation == Qt.Horizontal

                PropertyChanges {
                    target: track
                    height: privateStyle.sliderThickness
                    width: undefined
                    source: privateStyle.imagePath("qtg_fr_slider_h_track_normal")
                    border {
                        left: 20
                        right: 20
                        top: 0
                        bottom: 0
                    }
                }

                AnchorChanges {
                    target: track
                    anchors {
                        left: root.left
                        right: root.right
                        top: undefined
                        bottom: undefined
                        horizontalCenter: undefined
                        verticalCenter: root.verticalCenter
                    }
                }
            },
            State {
                name: "Vertical"
                when: orientation == Qt.Vertical

                PropertyChanges {
                    target: track
                    height: undefined
                    width: privateStyle.sliderThickness
                    source: privateStyle.imagePath("qtg_fr_slider_v_track_normal")
                    border {
                        left: 0
                        right: 0
                        top: 20
                        bottom: 20
                    }
                }

                AnchorChanges {
                    target: track
                    anchors {
                        left: undefined
                        right: undefined
                        top: root.top
                        bottom: root.bottom
                        horizontalCenter: root.horizontalCenter
                        verticalCenter: undefined
                    }
                }
            }
        ]

        anchors.leftMargin: platformStyle.paddingMedium
        anchors.rightMargin: platformStyle.paddingMedium
        anchors.topMargin: platformStyle.paddingMedium
        anchors.bottomMargin: platformStyle.paddingMedium

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

            Image {
                id: handle
                objectName: "handle"
                x: orientation == Qt.Horizontal ? (handleMouseArea.pressed ? x : model.position) : 0
                y: orientation == Qt.Horizontal ? 0 : (handleMouseArea.pressed ? y : model.position)

                sourceSize.height: platformStyle.graphicSizeSmall
                sourceSize.width: platformStyle.graphicSizeSmall

                anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                source: {
                    var handleIcon = "qtg_graf_slider_"
                        + (orientation == Qt.Horizontal ? "h" : "v")
                        + "_handle_"
                        + (handleMouseArea.pressed ? "pressed" : "normal")
                    privateStyle.imagePath(handleIcon)
                }

                MouseArea {
                    id: handleMouseArea
                    objectName: "handleMouseArea"

                    height: platformStyle.graphicSizeMedium
                    width: platformStyle.graphicSizeMedium
                    anchors.verticalCenter: orientation == Qt.Horizontal ? parent.verticalCenter : undefined
                    anchors.horizontalCenter: orientation == Qt.Horizontal ? undefined : parent.horizontalCenter

                    drag.target: handle
                    drag.axis: Drag.XandYAxis
                    drag.minimumX: orientation == Qt.Horizontal ? model.positionAtMinimum : 0
                    drag.maximumX: orientation == Qt.Horizontal ? model.positionAtMaximum : 0
                    drag.minimumY: orientation == Qt.Horizontal ? 0 : model.positionAtMinimum
                    drag.maximumY: orientation == Qt.Horizontal ? 0 : model.positionAtMaximum

                    onPositionChanged: {
                        if (updateValueWhileDragging) {
                            model.position = orientation == Qt.Horizontal ? handle.x : handle.y
                            valueIndicator.position()
                        }
                    }
                    onPressed: privateStyle.play(Symbian.BasicSlider)
                    onReleased: {
                        if (!updateValueWhileDragging)
                            model.position = orientation == Qt.Horizontal ? handle.x : handle.y
                        privateStyle.play(Symbian.BasicSlider)
                    }
                }
            }
        }

        Timer {
            id: keyActivity
            interval: 750
            onTriggered: track.keysActive = false
        }
    }

    Keys.onPressed: {
        internal.handleKeyEvent(event)
    }

    Component {
        id: valueIndicatorComponent
        ToolTip { text: root.valueIndicatorText == "" ? model.value : root.valueIndicatorText }
    }

    Loader {
        id: valueIndicator

        property int spacing: 2 * platformStyle.paddingLarge
        // Must match with the "maxWidth" padding defined in ToolTip
        property int toolTipPadding: platformStyle.paddingLarge
        sourceComponent: root.pressed && valueIndicatorVisible ? valueIndicatorComponent : undefined
        onLoaded: position()

        function position() {
            if (!valueIndicatorVisible || status != Loader.Ready)
                return

            var point = null
            if (orientation == Qt.Horizontal) {
                point = root.mapFromItem(track, handle.x + handle.width / 2 - valueIndicator.item.width / 2, 0)

                // Check if valueIndicator will be positioned beyond the right or
                // left boundaries and adjust if needed to keep it fully
                // visible on screen. In case the valueIndicator is so wide that it
                // does not fit the screen, it's positioned to left of the screen.
                var rightStop = screen.width - toolTipPadding
                var valueIndicatorLeftEdge = root.mapToItem(null, point.x, 0)
                var valueIndicatorRightEdge = root.mapToItem(null, point.x + valueIndicator.item.width, 0)

                if (valueIndicatorLeftEdge.x < toolTipPadding)
                    point.x = root.mapFromItem(null, toolTipPadding, 0).x
                else if (valueIndicatorRightEdge.x > rightStop)
                    point.x = root.mapFromItem(null, rightStop - valueIndicator.item.width, 0).x

                valueIndicator.item.x = point.x
                valueIndicator.item.y = point.y - valueIndicator.spacing - valueIndicator.item.height
            } else {
                point = root.mapFromItem(track, 0, handle.y + handle.height / 2 - valueIndicator.item.height / 2)
                valueIndicator.item.x = point.x - valueIndicator.spacing - valueIndicator.item.width
                valueIndicator.item.y = point.y
            }
        }
    }

    QtObject {
        id: internal

        function handleKeyEvent(keyEvent) {
            var oldValue = model.value
            if (orientation == Qt.Horizontal) {
                if (keyEvent.key == Qt.Key_Left) {
                    model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                } else if (keyEvent.key == Qt.Key_Right) {
                    model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                }
            } else { //Vertical
                if (keyEvent.key == Qt.Key_Up) {
                    model.value = inverted ? model.value + model.stepSize : model.value - model.stepSize
                } else if (keyEvent.key == Qt.Key_Down) {
                    model.value = inverted ? model.value - model.stepSize : model.value + model.stepSize
                }
            }
            if (oldValue != model.value)
                keyEvent.accepted = true
            if (keyEvent.accepted) {
                track.keysActive = true
                keyActivity.restart()
                valueIndicator.position()
            }
        }
    }
}
