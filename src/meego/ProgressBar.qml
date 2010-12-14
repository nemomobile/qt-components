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
import "UIConstants.js" as UI

ImplicitSizeItem {
    id: container

    property alias minimumValue: progressModel.minimumValue
    property alias maximumValue: progressModel.maximumValue
    property alias value: progressModel.value
    property bool indeterminate: false

    implicitWidth: UI.SIZE_BUTTON
    implicitHeight: 8

    BorderImage {
        id: background
        anchors.fill: parent
        source: "image://theme/meegotouch-progressindicator-bar-background"
        horizontalTileMode: BorderImage.Repeat
        border {
            top: 4
            bottom: 4
            left: 4
            right: 4
        }

        MaskedItem {
            id: bar

            property url texture: "image://theme/meegotouch-progressindicator-bar-known-texture"
            property int textureWidth

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            mask: BorderImage {
                width: bar.width
                height: bar.height
                source: "image://theme/meegotouch-progressindicator-bar-mask"
                border {
                    top: 4
                    bottom: 4
                    left: 4
                    right: 4
                }
            }

            visualItem: Image {
                width: bar.width + sourceSize.width
                height: bar.height
                source: bar.texture
                fillMode: Image.Tile
                onStatusChanged: if (status == Image.Ready) bar.textureWidth = sourceSize.width
            }

            states: [
                State {
                    name: "indeterminate"
                    when: indeterminate
                    PropertyChanges {
                        target: bar
                        imageOffset.x: -textureWidth
                        texture: "image://theme/meegotouch-progressindicator-bar-unknown-texture"
                        width: background.width
                    }
                    PropertyChanges {
                        target: candybarAnimation; running: true
                    }
                },
                State {
                    name: "progress"
                    when: !indeterminate
                    PropertyChanges {
                        target: bar
                        texture: "image://theme/meegotouch-progressindicator-bar-known-texture"
                        imageOffset.x: 0
                        width: progressModel.position
                    }
                    PropertyChanges {
                        target: candybarAnimation; running: false
                    }
                }
            ]

            NumberAnimation {
                id: candybarAnimation
                target: bar
                property: "imageOffset.x"
                from: -bar.textureWidth; to: 0
                duration: 1000 * bar.textureWidth / 40 // time = distance / speed, where speed = 40 from the style
                loops: Animation.Infinite
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
