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
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: container
    property alias minimumValue: progressModel.minimumValue
    property alias maximumValue: progressModel.maximumValue
    property alias value: progressModel.value
    property bool indeterminate: false

    implicitWidth: meegostyle.preferredWidth
    // The plankton theme seems to be using some other value inside libmeegotouch.
    // We stick to the preferred height, though.
    implicitHeight: meegostyle.preferredHeight

    property int __unknownBarWidth: Math.min(background.width, meegostyle.current.get("unknownBarSize").width)
    property bool __fullWidth: __unknownBarWidth >= container.width
    property int __unknownOffset: 0

    Style {
        id: meegostyle
        styleClass: "MProgressIndicatorStyle"
        styleType: "bar"
    }

    ScalableImage {
        id: background
        anchors.fill: parent
        style: meegostyle
        imageProperty: "progressBarBackground"
        clip: true

        MaskedImage {
            id: bar
            anchors.top: background.top
            anchors.bottom: background.bottom
            style: meegostyle
            maskProperty: "progressBarMask"
            fullWidth: __fullWidth

            states: [
                State {
                    name: "indeterminate"
                    when: indeterminate
                    PropertyChanges {
                        target: bar
                        imageProperty: "unknownBarTexture"
                        tiled: meegostyle.current.get("unknownBarTextureTiled")
                        x: 0
                        imageXOffset: 0
                        width: __unknownBarWidth
                    }
                    PropertyChanges {
                        target: candybarAnimation; running: __fullWidth
                    }
                    PropertyChanges {
                        target: oscillatingAnimation; running: !__fullWidth
                    }
                },
                State {
                    name: "progress"
                    when: !indeterminate
                    PropertyChanges {
                        target: bar
                        imageProperty: "knownBarTexture"
                        tiled: meegostyle.current.get("knownBarTextureTiled")
                        x: 0
                        imageXOffset: 0
                        width: progressModel.position
                    }
                    PropertyChanges {
                        target: candybarAnimation; running: false
                    }
                    PropertyChanges {
                        target: oscillatingAnimation; running: false
                    }
                }
            ]
        }

        MaskedImage {
            // We use a second image for the wrapping animation when not full width.
            id: wrappingBar
            anchors.top: background.top
            anchors.bottom: background.bottom
            anchors.right: background.right
            style: meegostyle
            imageProperty: "unknownBarTexture"
            maskProperty: "progressBarMask"
            tiled: meegostyle.current.get("unknownBarTextureTiled")
            imageXOffset: 0
            width: 0
            fullWidth: false
        }

        NumberAnimation {
            id: candybarAnimation
            target: bar
            property: "imageXOffset"
            from: 0; to: bar.implicitWidth
            duration: 1000 * bar.implicitWidth / meegostyle.current.get("speed")
            loops: Animation.Infinite
        }

        SequentialAnimation {
            id: oscillatingAnimation
            property real baseDuration: 1000 / 20 // from mprogressindicatorbarview.cpp
            loops: Animation.Infinite

            NumberAnimation {
                target: bar
                property: "x"
                property int maxValue: background.width - __unknownBarWidth - 1
                from: 0; to: maxValue
                duration: Math.max(0, oscillatingAnimation.baseDuration * maxValue)
            }
            PropertyAction {
                target: bar; property: "x"; value: 0
            }
            ParallelAnimation {
                id: wrapAroundAnimation
                property real wrapDuration: oscillatingAnimation.baseDuration * __unknownBarWidth
                NumberAnimation {
                    target: bar
                    property: "width"
                    from: 0; to: __unknownBarWidth
                    duration: wrapAroundAnimation.wrapDuration
                }
                NumberAnimation {
                    target: wrappingBar
                    property: "width"
                    from: __unknownBarWidth; to: 0
                    duration: wrapAroundAnimation.wrapDuration
                }
            }
        }
    }

    RangeModel {
        id: progressModel
        positionAtMinimum: 0
        positionAtMaximum: background.width

        // Defaults from MProgressIndicatorModel code
        minimumValue: 0
        maximumValue: 100
    }
}
