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
import Qt.labs.components 1.0 as QtComponents
import com.meego.themebridge 1.0 as ThemeBridge

Item {
    // The root item is meant as wrapper item than can stretch, but at
    // the same time, preserve aspect ratio of inner contents. In
    // addition, it will implement margins towards the groove
    id: root
    property bool vertical: false

    property alias value: valueModel.value
    property alias minimumValue: valueModel.minimumValue
    property alias maximumValue: valueModel.maximumValue
    property alias progress: receivedModel.value
    property alias steps: valueModel.steps

    property int indicatorPosition: vertical ? QtComponents.Orientation.Left : QtComponents.Orientation.Top
    property bool indicatorVisible: true

    property real innerLeftMargin: style.current.get("marginLeft")
    property real innerRightMargin: style.current.get("marginRight")
    property real innerTopMargin: style.current.get("marginTop")
    property real innerBottomMargin: style.current.get("marginBottom")

    property bool pressed: grooveMouseArea.pressed

    signal valueChanged(real value)
    signal pressed
    signal released

    width: innerLeftMargin + innerRightMargin + (vertical ? groove.width : style.current.get("groovePreferredLength"))
    height: innerTopMargin + innerBottomMargin + (vertical ? style.current.get("groovePreferredLength") : groove.height)

    QtComponents.RangeModel {
        // This model describes the range of values the slider can take
        // (minimum/maximum) and helps mapping between logical value and
        // the graphical position of the handle inside the component
        // (positionAtMinimum/positionAtMaximum)
        id: valueModel
        steps: 1
        positionAtMinimum: 0
        positionAtMaximum: root.vertical ? groove.height : groove.width
        onValueChanged: root.valueChanged(value)
    }

    QtComponents.RangeModel {
        // This model describes the progress/download percentage
        id: receivedModel
        steps: valueModel.steps
        positionAtMinimum: valueModel.positionAtMinimum
        positionAtMaximum: valueModel.positionAtMaximum
        minimumValue: valueModel.minimumValue
        maximumValue: valueModel.maximumValue
    }

    ThemeBridge.Style {
        id: style
        styleClass: "MSliderStyle"
    }

    ThemeBridge.ScalableImage {
        id: groove
        style: style
        imageProperty: vertical ? "backgroundVerticalBaseImage" : "backgroundBaseImage"

        anchors.left: vertical ? undefined : root.left
        anchors.right: vertical ? undefined : root.right
        anchors.top: vertical ? root.top : undefined
        anchors.bottom: vertical ? root.bottom : undefined
        anchors.horizontalCenter: vertical ? root.horizontalCenter : undefined
        anchors.verticalCenter: vertical ? undefined : root.verticalCenter

        anchors.leftMargin: root.innerLeftMargin
        anchors.rightMargin: root.innerRightMargin
        anchors.topMargin: root.innerTopMargin
        anchors.bottomMargin: root.innerBottomMargin

        ThemeBridge.ScalableImage {
            id: received
            style: style
            imageProperty: vertical ? "backgroundVerticalReceivedImage" : "backgroundReceivedImage"
            anchors.top: groove.top
            anchors.left: groove.left
            anchors.bottom: vertical ? undefined : groove.bottom
            anchors.right: vertical ? groove.right : undefined
            width: vertical ? undefined : receivedModel.position
            height: vertical ? receivedModel.position : undefined
        }

        ThemeBridge.ScalableImage {
            id: elapsed
            style: style
            imageProperty: vertical ? "backgroundVerticalElapsedImage" : "backgroundElapsedImage"
            anchors.top: groove.top
            anchors.left: groove.left
            anchors.bottom: vertical ? handlePixmap.verticalCenter : groove.bottom
            anchors.right: vertical ? groove.right : handlePixmap.horizontalCenter
        }

        ThemeBridge.Pixmap {
            // The size of this handle pixmap includes the reactive margins, which
            // makes it much larger than the visual size of the drawn image.
            id: handlePixmap
            style: style
            imageProperty: grooveMouseArea.pressed ?
                    (vertical ? "handleVerticalPressedPixmap" : "handlePressedPixmap")
                    : (vertical ? "handleVerticalPixmap" : "handlePixmap")
            anchors.verticalCenter: vertical ? undefined : parent.verticalCenter
            anchors.horizontalCenter: vertical ? parent.horizontalCenter : undefined

            x: valueModel.position - (width / 2)
            Behavior on x  {
                 PropertyAnimation {
                    // The animation data is taken from msliderview.cpp:
                    duration: grooveMouseArea.dragging ? 0 : 150
                    easing.type: Easing.OutSine
                }
            }

            y: valueModel.position - (height / 2)
            Behavior on y  {
                 PropertyAnimation {
                    // The animation data is taken from msliderview.cpp:
                    duration: grooveMouseArea.dragging ? 0 : 150
                    easing.type: Easing.OutSine
                }
            }
        }

        MouseArea {
            id: grooveMouseArea
            // RestrictedDragging means that the handle
            // should snap to steps _while_ dragging. This property is hidden
            // inside here as we see this as a style decition:
            property bool restrictedDragging: false

            // 'dragging' will be set to true when the user
            // is dragging on the the slider:
            property bool dragging: false

            anchors.verticalCenter: groove.verticalCenter
            anchors.horizontalCenter: groove.horizontalCenter
            width: handlePixmap.width + (vertical ? 0 : groove.width)
            height: handlePixmap.height + (vertical ? groove.height : 0)
            onPressed: {
                if (vertical) {
                    valueModel.position = mouseY - handlePixmap.height/2;
                    handlePixmap.y = restrictedDragging ?
                                valueModel.position - (handlePixmap.height / 2) : conformToRange(mouseY);
                } else {
                    valueModel.position = mouseX - handlePixmap.width/2;
                    handlePixmap.x = restrictedDragging ?
                                valueModel.position - (handlePixmap.width / 2) : conformToRange(mouseX);
                }
                root.pressed();
            }
            onPositionChanged: {
                dragging = true;
                if (vertical) {
                    valueModel.position = mouseY - (handlePixmap.height/2);
                    handlePixmap.y = restrictedDragging ?
                                valueModel.position - (handlePixmap.height / 2) : conformToRange(mouseY);
                } else {
                    valueModel.position = mouseX - (handlePixmap.width/2);
                    handlePixmap.x = restrictedDragging ?
                                valueModel.position - (handlePixmap.width / 2) : conformToRange(mouseX);
                }
            }
            onReleased: {
                dragging = false;
                if (vertical) {
                    handlePixmap.y = valueModel.position - (handlePixmap.height / 2)
                } else {
                    handlePixmap.x = valueModel.position - (handlePixmap.width / 2)
                }
                root.released();
            }

            function conformToRange(v) {
                return Math.min(valueModel.positionAtMaximum - (handlePixmap.width/2),
                         Math.max(valueModel.positionAtMinimum - (handlePixmap.width/2),
                                  v - handlePixmap.width));
            }
        }
     }

    ThemeBridge.ScalableImage {
        id: indicatorBackground
        style: labelStyle
        imageProperty: "backgroundImage"

        states: [
            State {
                when: indicatorPosition == QtComponents.Orientation.Left
                PropertyChanges { target: indicatorBackground; x: handlePixmap.x - width }
                PropertyChanges { target: indicatorBackground; y: handlePixmap.y - (height - handlePixmap.height)/2 }
            },
            State {
                when: indicatorPosition == QtComponents.Orientation.Top || indicatorPosition == QtComponents.Orientation.Undefined
                PropertyChanges { target: indicatorBackground; x: handlePixmap.x - (width - handlePixmap.width)/2 }
                PropertyChanges { target: indicatorBackground; y: handlePixmap.y - height }
            },
            State {
                when: indicatorPosition == QtComponents.Orientation.Right
                PropertyChanges { target: indicatorBackground; x: handlePixmap.x + handlePixmap.width }
                PropertyChanges { target: indicatorBackground; y: handlePixmap.y - (height - handlePixmap.height)/2 }
            },
            State {
                when: indicatorPosition == QtComponents.Orientation.Bottom
                PropertyChanges { target: indicatorBackground; x: handlePixmap.x - (width - handlePixmap.width)/2 }
                PropertyChanges { target: indicatorBackground; y: handlePixmap.y + handlePixmap.height }
            }
        ]

        ThemeBridge.Style {
            id: labelStyle
            styleClass: "MLabelStyle"
            styleObjectName: "MSliderHandleLabel"
        }

        Timer {
            // In the native meegotouch slider, the indicator
            // pops up 100ms after pressing the handle:
            id: indicatorTimer
            running: grooveMouseArea.pressed && indicatorVisible
            interval: 100;
            onTriggered: indicatorBackground.visible = grooveMouseArea.pressed && indicatorVisible
        }
        Binding {
            // ...but the indicator hides immidiatly:
            target: indicatorBackground
            property: "visible"
            value: grooveMouseArea.pressed && indicatorVisible
            when: indicatorBackground.visible
        }

        Text {
            id: indicatorText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: Math.round(valueModel.value * Math.pow(10, 1))/Math.pow(10, 1)

            // Set the width of indicatorBackground explicit circular
            // dependency (since we only allow the width to grow):
            onWidthChanged: indicatorBackground.width =
                            Math.max(indicatorBackground.width, width
                                     + labelStyle.current.get("paddingLeft")
                                     + labelStyle.current.get("paddingRight"))
        }
    }

}
