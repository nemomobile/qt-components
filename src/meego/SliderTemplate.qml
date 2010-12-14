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

import QtQuick 1.0
import Qt.labs.components 1.0

Item {
    id: slider

    //
    // Common API
    //
    property int orientation: Qt.Horizontal
    property alias minimumValue: range.minimumValue
    property alias maximumValue: range.maximumValue
    property bool updateValueWhileDragging: true
    property alias pressed: mouseArea.pressed
    property alias stepSize: range.stepSize

    // NOTE: this property is in/out, the user can set it, create bindings to it, and
    // at the same time the slider wants to update. There's no way in QML to do this kind
    // of updates AND allow the user bind it (without a Binding object). That's the
    // reason this is an alias to a C++ property in range model.
    property alias value: range.value


    //
    // Public extensions
    //
    property alias inverted: range.inverted
    property real secondaryValue

    // Value indicator displays the current value near the slider
    property bool valueIndicatorVisible: true
    property int valueIndicatorMargin: 10
    property string valueIndicatorPosition: __isVertical ? "Left" : "Top"

    // Reimplement this function to control how the value is shown in the
    // indicator.
    function formatValue(v) {
        return Math.round(v);
    }


    //
    // "Protected" properties
    //

    // Hooks for customizing the pieces of the slider
    property Item __grooveItem
    property Item __valueTrackItem
    property Item __secondaryValueTrackItem
    property Item __handleItem
    property Item __valueIndicatorItem

    property QtObject __range // for exposing positionForValue()

    property bool __isVertical: orientation == Qt.Vertical
    property int __preferredWidth: 400
    property int __preferredHeight: handle.height

    // Note that the indicatorText contains the value for the position the
    // handle is, which is not necessarily the available as the current
    // slider.value, since updateValueWhileDragging can be set to 'false'.
    property string __indicatorText: slider.formatValue(range.valueForPosition(handle.x))


    width: __isVertical ? __preferredHeight : __preferredWidth
    height: __isVertical ? __preferredWidth : __preferredHeight

    // This is a template slider, so every piece can be modified by passing a
    // different Component. The main elements in the implementation are
    //
    // - the 'range' does the calculations to map position to/from value,
    //   it also serves as a data storage for both properties;
    //
    // - the 'fakeHandle' is what the mouse area drags on the screen, it feeds
    //   the 'range' position and also reads it when convenient;
    //
    // - the real 'handle' it is the visual representation of the handle, that
    //   just follows the 'fakeHandle' position.
    //
    // When the 'updateValueWhileDragging' is false and we are dragging, we stop
    // feeding the range with position information, delaying until the next
    // mouse release.
    //
    // Everything is encapsulated in a contents Item, so for the
    // vertical slider, we just swap the height/width, make it
    // horizontal, and then use rotation to make it vertical again.

    Component.onCompleted: {
        __grooveItem.parent = groove;
        __valueTrackItem.parent = valueTrack;
        __secondaryValueTrackItem.parent = secondaryValueTrack;
        __handleItem.parent = handle;
        __valueIndicatorItem.parent = valueIndicator;
    }

    Item {
        id: contents

        width: __isVertical ? slider.height : slider.width
        height: __isVertical ? slider.width : slider.height
        rotation: __isVertical ? -90 : 0

        anchors.centerIn: slider

        RangeModel {
            id: range
            minimumValue: 0
            maximumValue: 100
            value: 0
            stepSize: 1.0

            positionAtMinimum: handle.width / 2
            positionAtMaximum: contents.width - handle.width / 2
        }

        Item {
            id: groove
            anchors.fill: parent
            anchors.leftMargin: handle.width / 2
            anchors.rightMargin: handle.width / 2
        }

        Item {
            id: secondaryValueTrack
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: groove.left

            width: {
                // ### This is a hack to make the binding system recalculate
                // the width, since it depends on those parameters, but the
                // dependecy is isolated inside C++ range model.
                range.positionAtMaximum; range.positionAtMinimum;
                range.minimumValue; range.maximumValue;
                return range.positionForValue(slider.secondaryValue) - handle.width / 2;
            }

            states: State {
                when: slider.inverted
                PropertyChanges {
                    target: secondaryValueTrack
                    width: {
                        // See comment above.
                        range.positionAtMaximum; range.positionAtMinimum;
                        range.minimumValue; range.maximumValue;
                        return groove.width - range.positionForValue(slider.secondaryValue) + handle.width / 2;
                    }
                }
                AnchorChanges {
                    target: secondaryValueTrack
                    anchors.left: undefined
                    anchors.right: groove.right
                }
            }
        }

        Item {
            id: valueTrack

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: groove.left
            anchors.right: handle.horizontalCenter
            anchors.rightMargin: handle.width / 2

            states: State {
                when: slider.inverted
                PropertyChanges {
                    target: valueTrack
                    anchors.rightMargin: 0
                    anchors.leftMargin: - handle.width / 2
                }
                AnchorChanges {
                    target: valueTrack
                    anchors.left: handle.horizontalCenter
                    anchors.right: groove.right
                }
            }
        }

        Item {
            id: handle
            transform: Translate { x: - handle.width / 2 }

            anchors.verticalCenter: parent.verticalCenter

            width: __handleItem.width
            height: __handleItem.height

            x: fakeHandle.x
            Behavior on x {
                id: behavior
                enabled: !mouseArea.drag.active

                PropertyAnimation {
                    duration: behavior.enabled ? 150 : 0
                    easing.type: Easing.OutSine
                }
            }
        }

        Item {
            id: fakeHandle
            width: handle.width
            height: handle.height
            transform: Translate { x: - handle.width / 2 }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            drag.target: fakeHandle
            drag.axis: Drag.XAxis
            drag.minimumX: range.positionAtMinimum
            drag.maximumX: range.positionAtMaximum

            onPressed: {
                // Clamp the value
                var newX = Math.max(mouse.x, drag.minimumX);
                newX = Math.min(newX, drag.maximumX);

                // Debounce the press: a press event inside the handler will not
                // change its position, the user needs to drag it.
                if (Math.abs(newX - fakeHandle.x) > handle.width / 2)
                    range.position = newX;
            }

            onReleased: {
                // If we don't update while dragging, this is the only
                // moment that the range is updated.
                if (!slider.updateValueWhileDragging)
                    range.position = fakeHandle.x;
            }
        }

        Item {
            id: valueIndicator

            transform: Translate { x: - handle.width / 2 }
            rotation: __isVertical ? 90 : 0
            visible: valueIndicatorVisible

            width: __valueIndicatorItem.width
            height: __valueIndicatorItem.height

            state: {
                if (!__isVertical)
                    return slider.valueIndicatorPosition;

                if (valueIndicatorPosition == "Right")
                    return "Bottom";
                if (valueIndicatorPosition == "Top")
                    return "Right";
                if (valueIndicatorPosition == "Bottom")
                    return "Left";

                return "Top";
            }

            anchors.margins: valueIndicatorMargin

            states: [
                State {
                    name: "Top"
                    AnchorChanges {
                        target: valueIndicator
                        anchors.bottom: handle.top
                        anchors.horizontalCenter: handle.horizontalCenter
                    }
                },
                State {
                    name: "Bottom"
                    AnchorChanges {
                        target: valueIndicator
                        anchors.top: handle.bottom
                        anchors.horizontalCenter: handle.horizontalCenter
                    }
                },
                State {
                    name: "Right"
                    AnchorChanges {
                        target: valueIndicator
                        anchors.left: handle.right
                        anchors.verticalCenter: handle.verticalCenter
                    }
                },
                State {
                    name: "Left"
                    AnchorChanges {
                        target: valueIndicator
                        anchors.right: handle.left
                        anchors.verticalCenter: handle.verticalCenter
                    }
                }
            ]
        }
    }

    // Range position normally follow fakeHandle, except when
    // 'updateValueWhileDragging' is false. In this case it will only follow
    // if the user is not pressing the handle.
    Binding {
        when: updateValueWhileDragging || !mouseArea.pressed
        target: range
        property: "position"
        value: fakeHandle.x
    }

    // During the drag, we simply ignore position set from the range, this
    // means that setting a value while dragging will not "interrupt" the
    // dragging activity.
    Binding {
        when: !mouseArea.drag.active
        target: fakeHandle
        property: "x"
        value: range.position
    }
}
