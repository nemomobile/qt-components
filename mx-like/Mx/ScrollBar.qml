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
    property bool vertical: false;
    property int stepIncrement: 1
    property int pageIncrement: 10
    property int pageSize: 10

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

        MouseArea {
            anchors.fill: parent
            onPressed: {
                handleRelease();
                scrollbar.scrollStart();
            }
            onReleased: {
                scrollbar.scrollStop();
            }

            function handleRelease() {
                if (!scrollbar.vertical) {
                    handle.x = handle.x + ( mouseX > handle.x ? pageIncrement : -1 * pageIncrement);
                } else {
                    handle.y = handle.y + ( mouseY > handle.y ? pageIncrement : -1 * pageIncrement);
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
        }

        state: "horizontal";
        states: [
            State {
                name: "horizontal"
                AnchorChanges {
                    target: scrollbarPath
                    left: button1.right
                    right: button2.left
                }
                AnchorChanges {
                    target: button1
                    left: scrollbar.left
                }
                AnchorChanges {
                    target: button2
                    right: scrollbar.right
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

                    border.left: 10
                    border.right: 10

                    source: "images/scroll-hhandle"
                    + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png"
                }

                PropertyChanges {
                    target: button1;
                    source: "images/scroll-button-left"
                          + (button1MouseRegion.containsMouse ? "-hover" : "") + ".png"
                }
                PropertyChanges {
                    target: button2;
                    source: "images/scroll-button-right"
                          + (button2MouseRegion.containsMouse ? "-hover" : "") + ".png"
                }
            },
            State {
                name: "vertical"
                when: scrollbar.vertical
                AnchorChanges {
                    target: scrollbarPath
                    top: button1.bottom
                    bottom: button2.top
                }
                AnchorChanges {
                    target: button1
                    top: scrollbar.top
                }
                AnchorChanges {
                    target: button2
                    bottom: scrollbar.bottom
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

                    border.top: 10
                    border.bottom: 10

                    source: "images/scroll-vhandle"
                    + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png"
                }
                PropertyChanges {
                    target: button1;
                    source: "images/scroll-button-up"
                          + (button1MouseRegion.containsMouse ? "-hover" : "") + ".png"
                }
                PropertyChanges {
                    target: button2;
                    source: "images/scroll-button-down"
                          + (button2MouseRegion.containsMouse ? "-hover" : "") + ".png"
                }
            }
        ]
    }

    Image {
        id: button1

        // XXX That shouldn't be necessary as I'm already setting the
        //     source in the state. My impression is that once the Image
        //     is created without source (and size) it will never have a
        //     proper size.
        source: "images/scroll-button-left.png";

        MouseArea {
            id: button1MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                if (scrollbarPath.state == "horizontal") {
                    if (handle.x >= stepIncrement)
                        handle.x = handle.x - stepIncrement;
                } else {
                    if (handle.y >= stepIncrement)
                        handle.y = handle.y - stepIncrement;
                }
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }
        }
    }

    Image {
        id: button2

        // XXX That shouldn't be necessary as I'm already setting the
        //     source in the state. My impression is that once the Image
        //     is created without source (and size) it will never have a
        //     proper size.
        source: "images/scroll-button-right.png";

        MouseArea {
            id: button2MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onPressed: {
                if (scrollbarPath.state == "horizontal") {
                    if (handle.x + handle.width <= scrollbarPath.width - stepIncrement)
                        handle.x = handle.x + stepIncrement;
                } else {
                    if (handle.y + handle.height <= scrollbarPath.height - stepIncrement)
                        handle.y = handle.y + stepIncrement;
                }
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }
        }
    }
}
