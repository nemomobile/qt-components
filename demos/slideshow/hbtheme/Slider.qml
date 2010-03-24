// hb theme slider

import Qt 4.6
import Components 1.0

import "../common/model"

Item {
    id: slider
    property alias value: model.value
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    width: 300; height: 100
    Item {
        id: background
        x: 0
        y: (parent.height - height) / 2
        width: parent.width
        BorderImage {
            id: background_normal
            x: 0
            y: (parent.height - height) / 2
            width: parent.width
            border.left: 10;
            border.right: 10;
            source: "images/slider-bg.png"
            visible: !mouseArea.pressed;
        }
        BorderImage {
            id: background_pressed
            x: 0
            y: (parent.height - height) / 2
            width: parent.width
            border.left: 10;
            border.right: 10;
            source: "images/slider-bg-pressed.png"
            visible: mouseArea.pressed;
        }
        BorderImage {
            id: elapsed
            source: "images/slider-filled.png"
            x: 0
            y: (parent.height - height) / 2
            width: knob.x + (knob.width / 2)
            border.left: 10;
            border.right: 10;
        }
        Item {
            id: knob
            x: 0 - knob.width / 2;
            y: (parent.height - knob_normal.height) / 2
            width: knob_normal.width
            height: knob_normal.height
            Image {
                id: knob_normal
                source: "images/slider-handle.svg"
                visible: !mouseArea.pressed;
            }
            Image {
                id: knob_pressed
                source: "images/slider-handle-pressed.svg"
                visible: mouseArea.pressed;
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                drag.target: parent
                drag.axis: "XAxis"
                drag.minimumX: -knob.width / 2
                drag.maximumX: background.width - (knob.width / 2)
            }
        }
    }

    MyRangeModel {
        id: model
        maximumPosition: background.width - knob.width
    }
}
