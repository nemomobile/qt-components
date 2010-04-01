/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.7

Item {
    id: scrollbar

    signal scrollStart
    signal scrollStop
    state: "horizontal"
    property real value: currentValue()
    property bool vertical: false
    property int stepIncrement: 1
    property int pageIncrement: 10
    property int pageSize: 10
    property int lower: 0
    property int upper: scrollbarPath.maximum
    property int documentSize: 100
    property int viewSize: 20

    height: 50
    width: 50

    function currentValue() {
        if (!vertical)
            return handle.x / (scrollbarPath.width - handle.width);
        else
            return handle.y / (scrollbarPath.height - handle.height);
    }

    Rectangle  {
        id: scrollbarPath

        color: '#dee2e5'
        property int maximum: (scrollbar.vertical == false ? width : height)
        property bool hold: false

        Timer {
            interval: 50
            repeat: true
            running: scrollbarPath.hold
            onTriggered: { scrollbarPathMouseRegion.handleRelease(); }
        }

        MouseArea {
            id: scrollbarPathMouseRegion
            anchors.fill: parent
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
                if (!scrollbar.vertical) {
                    var newx = handle.x + ( mouseX > handle.x ? pageIncrement : -1 * pageIncrement);
                    if (newx < scrollbar.lower)
                        handle.x = scrollbar.lower
                    else if (newx + handle.width> scrollbar.upper)
                        handle.x = scrollbar.upper - handle.width
                    else
                        handle.x = newx
                } else {
                    var newy = handle.y + ( mouseY > handle.y ? pageIncrement : -1 * pageIncrement);
                    if (newy < scrollbar.lower)
                        handle.y = scrollbar.lower
                    else if (newy + handle.height > scrollbar.upper)
                        handle.y = scrollbar.upper - handle.height
                    else
                        handle.y = newy
                }
            }
        }

        BorderImage {
            id: handle

            Behavior on x {
                PropertyAnimation { easing.type: "OutCubic"; duration: 250; }
            }

            Behavior on y {
                PropertyAnimation { easing.type: "OutCubic";  duration: 250; }
            }

            MouseArea {
                id: handleMouseRegion
                anchors.fill: parent
                hoverEnabled: true
                drag.target: handle

                onPressed: { scrollbar.scrollStart(); }
                onReleased: { scrollbar.scrollStop(); }
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
                    target: scrollbarPath
                    height: button1.height
                }
                PropertyChanges {
                    target: handleMouseRegion
                    drag.axis: "XAxis"
                    drag.minimumX: 0
                    drag.maximumX: scrollbarPath.width - handle.width
                }

                PropertyChanges {
                    target: handle
                    x: value
                    width: scrollbar.width * viewSize / documentSize; // ### bound
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
                    target: scrollbarPath
                    width: button1.width
                }
                PropertyChanges {
                    target: handleMouseRegion
                    drag.axis: "YAxis"
                    drag.minimumY: 0
                    drag.maximumY: scrollbarPath.height - handle.height
                }
                PropertyChanges {
                    target: handle
                    y: value
                    height: scrollbar.height * viewSize / documentSize; // ###bound
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
            interval: 100
            repeat: true;
            running: button1.hold

            onTriggered: { button1MouseRegion.buttonPressed(); }
        }

        MouseArea {
            id: button1MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                button1.hold = true;
                buttonPressed();
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }

            onReleased: {
                button1.hold = false;
            }

            function buttonPressed() {
                if (scrollbarPath.state == "horizontal") {
                    if (handle.x >= stepIncrement)
                        handle.x = handle.x - stepIncrement;
                    else
                        handle.x = lower;
                } else {
                    if (handle.y >= stepIncrement)
                        handle.y = handle.y - stepIncrement;
                    else
                        handle.y = lower;
                }
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
            interval: 50
            repeat: true
            running: button2.hold
            onTriggered: { button2MouseRegion.buttonPressed(); }
        }

        MouseArea {
            id: button2MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                button2.hold = true;
                buttonPressed();
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }

            onReleased: {
                button2.hold = false;
            }

            function buttonPressed() {
                if (scrollbarPath.state == "horizontal") {
                    if (handle.x + handle.width <= scrollbarPath.width - stepIncrement)
                        handle.x = handle.x + stepIncrement;
                    else
                        handle.x = upper - handle.width;
                } else {
                    if (handle.y + handle.height <= scrollbarPath.height - stepIncrement)
                        handle.y = handle.y + stepIncrement;
                    else
                        handle.y = upper - handle.height;
                }
            }
        }
    }
}
