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
import Qt.labs.components 1.0
import "." 1.0

ImplicitSizeItem {
    id: root

    // Common Public API
    property alias minimumValue: model.minimumValue
    property alias maximumValue: model.maximumValue
    property alias value: model.value
    property bool indeterminate: false

    implicitWidth: Math.max(50, screen.width / 2) // TODO: use screen.displayWidth
    implicitHeight: privateStyle.sliderThickness

    BorderImage {
        id: background
        source: privateStyle.imagePath("qtg_fr_progressbar_track")
        border { left: 20; top: 0; right: 20; bottom: 0 }

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        width: parent.width
        height: parent.height

        states: [
            State {
                name: "determinate"
                when: indeterminate == false
                PropertyChanges {
                    target: frame
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateMaskedImage
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateMaskedImageExtra
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    running: false
                }
            },
            State {
                name: "indeterminate"
                when: indeterminate == true
                PropertyChanges {
                    target: frame
                    visible: false
                }
                PropertyChanges {
                    target: indeterminateMaskedImage
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateMaskedImageExtra
                    visible: true
                }
                PropertyChanges {
                    target: indeterminateAnimation
                    running: true
                }
            }
        ]

        ParallelAnimation {
            id: indeterminateAnimation
            loops: Animation.Infinite
            running: true

            PropertyAnimation { target: indeterminateMaskedImage; property: "offset.x"; from: height; to: 1; easing.type: Easing.Linear; duration: privateStyle.sliderThickness * 25 }
            PropertyAnimation { target: indeterminateMaskedImageExtra; property: "offset.x"; from: 0; to: 1 - height; easing.type: Easing.Linear; duration: privateStyle.sliderThickness * 25 }
        }

        Item {
            clip: true

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: model.position

            BorderImage {
                id: frame
                source: privateStyle.imagePath("qtg_fr_progressbar_fill")
                border { left: 20; top: 0; right: 20; bottom: 0 }

                anchors.left: parent.left
                anchors.top: parent.top

                width: root.width
                height: parent.height
            }
        }

        MaskedImage {
            id: indeterminateMaskedImage
            anchors.fill: parent

            topMargin: 0
            bottomMargin: 0
            leftMargin: 20
            rightMargin: 20

            tiled: true
            imageName: "qtg_graf_progressbar_wait"
            maskName: "qtg_fr_progressbar_mask"
        }

        // Secondary tile to keep the bar full when the animation scrolls
        MaskedImage {
            id: indeterminateMaskedImageExtra
            anchors.fill: parent

            topMargin: 0
            bottomMargin: 0
            leftMargin: 20
            rightMargin: 20

            tiled: false
            imageName: "qtg_graf_progressbar_wait"
            maskName: "qtg_fr_progressbar_mask"
        }
    }

    RangeModel {
        id: model
        minimumValue: 0.0
        maximumValue: 1.0
        positionAtMinimum: 0.0
        positionAtMaximum: background.width
    }
}

