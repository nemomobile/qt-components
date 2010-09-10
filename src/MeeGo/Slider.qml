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

    property bool handleDragging: invisibleHandleMouseArea.drag.active
    property bool handlePressed: invisibleHandleMouseArea.pressed

    // restrictedDragging means that the handle
    // should snap to steps _while_ dragging
    property bool restrictedDragging: false

    property real innerLeftMargin: style.get("marginLeft")
    property real innerRightMargin: style.get("marginRight")
    property real innerTopMargin: style.get("marginTop")
    property real innerBottomMargin: style.get("marginBottom")

    width: innerLeftMargin + innerRightMargin + (vertical ? groove.width : style.get("groovePreferredLength"))
    height: innerTopMargin + innerBottomMargin + (vertical ? style.get("groovePreferredLength") : groove.height)

    signal valueChanged(real value)
    signal handlePressed
    signal handleReleased

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

        MouseArea {
            id: grooveMouseArea
            // We get empty values when asking for reactive margins from the style.
            // So for now, use the size of the handle instead:
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: handlePixmap.width + (vertical ? 0 : parent.width)
            height: handlePixmap.height + (vertical ? parent.height : 0)
            onPressed: valueModel.position = vertical ? mouseY - (handlePixmap.height/2) : mouseX - (handlePixmap.width/2)
        }

        ThemeBridge.Pixmap {
            // The size of this handle pixmap includes the reactive margins, which
            // makes it much larger than the visual size of the drawn image.
            id: handlePixmap
            style: style
            imageProperty: invisibleHandleMouseArea.pressed ?
                    (vertical ? "handleVerticalPressedPixmap" : "handlePressedPixmap")
                    : (vertical ? "handleVerticalPixmap" : "handlePixmap")
            anchors.verticalCenter: vertical ? undefined : parent.verticalCenter
            anchors.horizontalCenter: vertical ? parent.horizontalCenter : undefined

            // Let the handle pixmap follow the model or the drag
            // area, depending on the restrictedDragging property:
            x: vertical ? 0 : (invisibleHandleMouseArea.pressed && !restrictedDragging ?
                     handleDragItem.x : valueModel.position - (width / 2))
            y: vertical ? (invisibleHandleMouseArea.pressed && !restrictedDragging ?
                           handleDragItem.y : valueModel.position - (height / 2)) : 0

            Behavior on x  {
                // Animate the handle pixmap along the groove when the handle
                // changes position, unless the user is dragging the handleDragItem
                PropertyAnimation {
                    // The animation data is taken from msliderview.cpp:
                    duration: invisibleHandleMouseArea.pressed ? 0 : 150
                    easing.type: Easing.OutSine
                }
            }
            Behavior on y {
                // Animate the handle pixmap along the groove when the handle
                // changes position, unless the user is dragging the handleDragItem
                PropertyAnimation {
                    // The animation data is taken from msliderview.cpp:
                    duration: invisibleHandleMouseArea.pressed ? 0 : 150
                    easing.type: Easing.OutSine
                }
            }

            ThemeBridge.Pixmap {
                id: handleIndicator
                style: style
                visible: false
                imageProperty: "handleLabelArrowDownPixmap"
                anchors.horizontalCenter: handlePixmap.horizontalCenter
                width: 50; height: 50
                y: -height
            }
        }

        Item {
            // The purpose of this item is to give the user an item to drag on, but at
            // the same time, restrict the _visible_ handlePixmap to valid values in the model
            // (typically discrete steps on the groove). So this item is transparent to make
            // it look like the user is dragging the actual handle.
            id: handleDragItem
            // When the user is not using the handle, we want the handleDragItem to
            // aling with the pixmap handle, so there is a 1:1 mapping between them
            x: invisibleHandleMouseArea.pressed ? x : handlePixmap.x
            y: invisibleHandleMouseArea.pressed ? y : handlePixmap.y
            width: handlePixmap.width
            height: handlePixmap.height
            anchors.verticalCenter: vertical ? undefined : parent.verticalCenter
            anchors.horizontalCenter: vertical ? parent.horizontalCenter : undefined

            MouseArea {
                id: invisibleHandleMouseArea
                anchors.fill: parent
                drag.axis: vertical ? Drag.YAxis : Drag.XAxis
                drag.minimumX: 0 - width/2
                drag.minimumY: 0 - height/2
                drag.maximumX: groove.width - width/2
                drag.maximumY: groove.height - height/2
                drag.target: handleDragItem
                onPositionChanged: valueModel.position = vertical ?
                    (handleDragItem.y + (height/2)) : (handleDragItem.x + (width/2))
                // The effective position returned by the model is constrained to steps and
                // ranges, and might therefore differ from the position set explicit. To let
                // UI and model be in sync, we just set the visual postion before the dragging starts:
                onPressed: { valueModel.position = valueModel.position; root.handlePressed() }
                onReleased: { valueModel.position = valueModel.position; root.handleReleased() }
            }
        }
    }
}
