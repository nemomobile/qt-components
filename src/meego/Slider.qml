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
import com.meego.themebridge 1.0 as ThemeBridge

SliderTemplate {
    id: slider

    ThemeBridge.Style {
        id: sliderStyle
        styleClass: "MSliderStyle"
    }

    // TODO: feedback while moving (changing position)...

    onPressedChanged: {
        if (pressed) {
            sliderStyle.feedback("pressFeedback");
        } else {
            sliderStyle.feedback("releaseFeedback");
        }
    }

    __handleItem: Image {
        source: slider.pressed ?
                    "image://theme/meegotouch-slider-handle-background-pressed-horizontal" :
                    "image://theme/meegotouch-slider-handle-background-horizontal"
    }

    __grooveItem: BorderImage {
            source: "image://theme/meegotouch-slider-background-horizontal"
            border { left: 4; top: 4; right: 4; bottom: 4 }
            height: 12

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
    }

    __valueTrackItem: BorderImage {
        source: "image://theme/meegotouch-slider-elapsed-background-horizontal"
        border { left: 4; top: 4; right: 4; bottom: 4 }
        height: 12

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
    }

    __secondaryValueTrackItem: BorderImage {
        source: "image://theme/meegotouch-slider-received-background-horizontal"
        border { left: 4; top: 4; right: 4; bottom: 4 }
        height: 12

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
    }

    __valueIndicatorItem: BorderImage {
        id: indicatorBackground
        source: "image://theme/meegotouch-slider-handle-value-background"
        border { left: 12; top: 12; right: 12; bottom: 12 }

        width: label.width + 50
        height: 80

        Image {
            id: arrow
        }

        state: slider.valueIndicatorPosition
        states: [
            State {
                name: "Top"
                PropertyChanges {
                    target: arrow
                    source: "image://theme/meegotouch-slider-handle-label-arrow-down"
                }
                AnchorChanges {
                    target: arrow
                    anchors.top: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            },
            State {
                name: "Bottom"
                PropertyChanges {
                    target: arrow
                    source: "image://theme/meegotouch-slider-handle-label-arrow-up"
                }
                AnchorChanges {
                    target: arrow
                    anchors.bottom: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            },
            State {
                name: "Left"
                PropertyChanges {
                    target: arrow
                    source: "image://theme/meegotouch-slider-handle-label-arrow-right"
                }
                AnchorChanges {
                    target: arrow
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            },
            State {
                name: "Right"
                PropertyChanges {
                    target: arrow
                    source: "image://theme/meegotouch-slider-handle-label-arrow-right"
                }
                AnchorChanges {
                    target: arrow
                    anchors.right: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        ]

        Label {
            id: label
            anchors.centerIn: parent
            text: slider.__indicatorText
        }

        // Native libmeegotouch slider value indicator pops up 100ms after pressing
        // the handle... but hiding happens without delay.
        visible: slider.valueIndicatorVisible && slider.pressed
        Behavior on visible {
            enabled: !indicatorBackground.visible
            PropertyAnimation {
                duration: 100
            }
        }
    }
}
