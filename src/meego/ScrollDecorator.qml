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

Item {
    id: scrollDecorator
    property Flickable flickable
    anchors.fill: flickable
    z: flickable.z + 1;

    Style {
        id: meegostyle
        styleClass: "MPositionIndicatorStyle"
    }
    property real __minIndicatorSize: meegostyle.current.get("minIndicatorSize")
    property real __hideTimeout: meegostyle.current.get("hideTimeout")
    property bool __firstTime: true

    Timer {
        // Hack to have the indicators flash when the view is shown the first time.
        // Ideally we would wait until the Flickable is complete, but it doesn't look
        // possible given the current limitations of QML.
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            // Will trigger a "visible" to "hidden" state change
            __firstTime = false;
        }
    }

    Component {
        id: hiddenStateTransitions


        QtObject {
            property Transition toHidden: Transition {
                from: "visible"; to: "hidden"
                SequentialAnimation {
                    PauseAnimation {
                        duration: __hideTimeout
                    }
                    NumberAnimation {
                        properties: "opacity"
                    }
                }
            }

            property Transition fromHidden: Transition {
                from: "hidden"; to: "visible"
                NumberAnimation {
                    properties: "opacity"
                }
            }

            property variant trList: [ toHidden, fromHidden ]
        }
    }

    Item {
        id: verticalSD
        property bool shouldShow: flickable.height > 0 && flickable.contentHeight > flickable.height
        opacity: 0
        height: parent.height
        anchors.right: parent.right

        ScalableImage {
            id: verticalBG
            style: meegostyle
            imageProperty: "backgroundImage"
            height: parent.height
            anchors.right: parent.right
        }
        ScalableImage {
            style: meegostyle
            imageProperty: "indicatorImage"

            ScrollDecoratorSizer {
                id: vSizer
                minSize: __minIndicatorSize
                maxSize: horizontalSD.shouldShow
                         ? flickable.height - horizontalBG.implicitWidth
                         : flickable.height
                positionRatio: flickable.visibleArea.yPosition
                sizeRatio: flickable.visibleArea.heightRatio
            }

            y: vSizer.position
            height: vSizer.size
            anchors.right: parent.right
        }

        states: [
            State {
                name: "visible"
                when: verticalSD.shouldShow && (__firstTime || flickable.moving)
                PropertyChanges {
                    target: verticalSD
                    opacity: 1
                }
            },

            State {
                name: "hidden"
                when: (!verticalSD.shouldShow || !flickable.moving) && !__firstTime
                PropertyChanges {
                    target: verticalSD
                    opacity: 0
                }
            }
        ]

        transitions: hiddenStateTransitions.createObject(verticalSD).trList
    }

    Item {
        // Workaround for some themes that don't have square images
        id: horizontalSD
        property bool shouldShow: flickable.width > 0 && flickable.contentWidth > flickable.width
        opacity: 0
        anchors.fill: parent

        Item {
            x: flickable.height - flickable.width
            width: flickable.width
            height: parent.height
            transformOrigin: Item.BottomRight
            rotation: -90

            ScalableImage {
                id: horizontalBG
                style: meegostyle
                imageProperty: "backgroundImage"
                y: 0
                height: flickable.width
                anchors.left: parent.right
            }
            ScalableImage {
                style: meegostyle
                imageProperty: "indicatorImage"

                ScrollDecoratorSizer {
                    id: hSizer
                    minSize: __minIndicatorSize
                    maxSize: verticalSD.shouldShow
                             ? flickable.width - verticalBG.implicitWidth
                             : flickable.width
                    positionRatio: flickable.visibleArea.xPosition
                    sizeRatio: flickable.visibleArea.widthRatio
                }

                y: hSizer.position
                height: hSizer.size
                anchors.left: parent.right
            }
        }

        states: [
            State {
                name: "visible"
                when: horizontalSD.shouldShow && (__firstTime || flickable.moving)
                PropertyChanges {
                    target: horizontalSD
                    opacity: 1
                }
            },
            State {
                name: "hidden"
                when: (!horizontalSD.shouldShow || !flickable.moving) && !__firstTime
                PropertyChanges {
                    target: horizontalSD
                    opacity: 0
                }
            }
        ]

        transitions: hiddenStateTransitions.createObject(horizontalSD).trList
    }
}
