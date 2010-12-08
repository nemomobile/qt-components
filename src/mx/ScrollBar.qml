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
    id: scrollbar

    signal scrollStart
    signal scrollStop

    property alias value: model.value
    property alias inverted: model.inverted
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    property alias singleStep: model.singleStep
    property alias pageStep: model.pageStep

    property bool vertical: false
    property int documentSize: 100
    property int viewSize: 20
    property alias tooltipText: tooltip.text

    height: 50
    width: 50

    function addSingleStep() {
        // dont use the convenience API to trigger the
        // behavior on value specified on rangemodel
        if (!inverted)
            model.value += model.singleStep;
        else
            model.value -= model.singleStep;
    }

    function subSingleStep() {
        // dont use the convenience API to trigger the
        // behavior on value specified on rangemodel
        if (!inverted)
            model.value -= model.singleStep;
        else
            model.value += model.singleStep;
    }

    function addPageStep() {
        // dont use the convenience API to trigger the
        // behavior on value specified on rangemodel
        if (!inverted)
            model.value += model.pageStep;
        else
            model.value -= model.pageStep;
    }

    function subPageStep() {
        // dont use the convenience API to trigger the
        // behavior on value specified on rangemodel
        if (!inverted)
            model.value -= model.pageStep;
        else
            model.value += model.pageStep;
    }

    BorderImage {
        id: scrollbarPath
        source: Qt.resolvedUrl("images/scroll-hbackground.png")

        border.left: 8;
        border.top: 8
        border.right: 8;
        border.bottom: 8

        property bool hold: false

        Timer {
            interval: 150
            repeat: true
            running: scrollbarPath.hold
            onTriggered: { scrollbarPathMouseRegion.handleRelease(); }
        }

        MouseArea {
            id: scrollbarPathMouseRegion
            anchors.fill: parent
            hoverEnabled: tooltip.text
            onPressed: {
                scrollbarPath.hold = true;
                handleRelease();
                scrollbar.scrollStart();
            }
            onReleased: {
                scrollbarPath.hold = false;
                scrollbar.scrollStop();
            }

            function handleRelease() {
                var pos;
                if (!scrollbar.vertical) {
                    if (mouseX > (handle.x + handle.width)) {
                        addPageStep();
                    } else {
                        subPageStep();
                    }
                } else {
                    if (mouseY > (handle.y + handle.height)) {
                        addPageStep();
                    } else {
                        subPageStep();
                    }
                }
            }
        }

        BorderImage {
            id: handle

            MouseArea {
                id: handleMouseRegion
                anchors.fill: parent
                hoverEnabled: true
                drag.target: handle

                onPressed: { scrollbar.scrollStart(); }
                onReleased: { scrollbar.scrollStop(); }
            }

            function handleSize() {
                var size1;
                if (!scrollbar.vertical)
                    size1 = scrollbarPath.width;
                else
                    size1 = scrollbarPath.height;

                var size = (size1 * viewSize) / documentSize;
                if (size < 0)
                    return 0;
                if (size > size1)
                    return size1;

                return size;
            }
        }

        state: "horizontal";
        states: [
            State {
                name: "horizontal"
                AnchorChanges {
                    target: scrollbarPath
                    anchors.left: button1.right
                    anchors.right: button2.left
                }
                AnchorChanges {
                    target: button1
                    anchors.left: scrollbar.left
                }
                AnchorChanges {
                    target: button2
                    anchors.right: scrollbar.right
                }
                PropertyChanges {
                    target: scrollbar;
                    height: button1.height;
                }
                PropertyChanges {
                    target: scrollbarPath
                    height: button1.height
                    source: Qt.resolvedUrl("images/scroll-hbackground.png")
                }
                PropertyChanges {
                    target: handleMouseRegion
                    drag.axis: "XAxis"
                    drag.minimumX: 0
                    drag.maximumX: scrollbarPath.width - handle.width
                }
                PropertyChanges {
                    target: model
                    position: handle.x
                    positionAtMaximum: scrollbarPath.width - handle.width
                }
                PropertyChanges {
                    target: handle
                    x: model.position
                    width: handleSize()
                    border.left: 10
                    border.right: 10
                    source: Qt.resolvedUrl("images/scroll-hhandle"
                                           + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
                PropertyChanges {
                    target: button1;
                    source: Qt.resolvedUrl("images/scroll-button-left"
                                           + (button1MouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
                PropertyChanges {
                    target: button2;
                    source: Qt.resolvedUrl("images/scroll-button-right"
                                           + (button2MouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
            },
            State {
                name: "vertical"
                when: scrollbar.vertical
                AnchorChanges {
                    target: scrollbarPath
                    anchors.top: button1.bottom
                    anchors.bottom: button2.top
                }
                AnchorChanges {
                    target: button1
                    anchors.top: scrollbar.top
                }
                AnchorChanges {
                    target: button2
                    anchors.bottom: scrollbar.bottom
                }
                PropertyChanges {
                    target: scrollbar;
                    width: button1.width;
                }
                PropertyChanges {
                    target: scrollbarPath
                    width: button1.width
                    source: Qt.resolvedUrl("images/scroll-vbackground.png")
                }
                PropertyChanges {
                    target: handleMouseRegion
                    drag.axis: "YAxis"
                    drag.minimumY: 0
                    drag.maximumY: scrollbarPath.height - handle.height
                }
                PropertyChanges {
                    target: model
                    position: handle.y
                    positionAtMaximum: scrollbarPath.height - handle.height
                }
                PropertyChanges {
                    target: handle
                    y: model.position
                    height: handleSize()
                    border.top: 10
                    border.bottom: 10
                    source: Qt.resolvedUrl("images/scroll-vhandle"
                                           + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
                PropertyChanges {
                    target: button1;
                    source: Qt.resolvedUrl("images/scroll-button-up"
                                           + (button1MouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
                PropertyChanges {
                    target: button2;
                    source: Qt.resolvedUrl("images/scroll-button-down"
                                           + (button2MouseRegion.containsMouse ? "-hover" : "") + ".png")
                }
            }
        ]
    }

    /*
     * ### Yes, button1 and button2 have too much code repeated...
     */
    Image {
        id: button1
        property bool hold: false

        // XXX That shouldn't be necessary as I'm already setting the
        //     source in the state. My impression is that once the Image
        //     is created without source (and size) it will never have a
        //     proper size.
        source: "images/scroll-button-left.png"

        Timer {
            interval: 150
            repeat: true;
            running: button1.hold
            onTriggered: { subSingleStep(); }
        }

        MouseArea {
            id: button1MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                button1.hold = true;
                subSingleStep();
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }

            onReleased: {
                button1.hold = false;
            }
        }
    }

    Image {
        id: button2
        property bool hold: false;

        // XXX That shouldn't be necessary as I'm already setting the
        //     source in the state. My impression is that once the Image
        //     is created without source (and size) it will never have a
        //     proper size.
        source: "images/scroll-button-right.png"

        Timer {
            interval: 150
            repeat: true
            running: button2.hold
            onTriggered: { addSingleStep(); }
        }

        MouseArea {
            id: button2MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                button2.hold = true;
                addSingleStep();
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }

            onReleased: {
                button2.hold = false;
            }
        }
    }

    RangeModel {
        id: model

        Behavior on value {
            PropertyAnimation {
                easing.type: "OutCubic";
                duration: 250;
            }
        }

        singleStep: 1
        pageStep: 10
        minimumValue: 0
        maximumValue: 100
        positionAtMinimum: 0
    }

    TooltipLoader {
        id: tooltip;
        anchors.fill: parent;

        property bool pressDismiss: false;
        property bool containsMouse: scrollbarPathMouseRegion.containsMouse
                                  || handleMouseRegion.containsMouse
                                  || button1MouseRegion.containsMouse
                                  || button2MouseRegion.containsMouse
        property bool mousePressed: scrollbarPathMouseRegion.pressed
                                 || handleMouseRegion.pressed
                                 || button1MouseRegion.pressed
                                 || button2MouseRegion.pressed;
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
