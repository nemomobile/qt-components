// hb theme slider

import Qt 4.6
import Components 1.0

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
            width: knob.x + knob.width
            border.left: 10;
            border.right: 10;
        }
        Item {
            id: knob
            x: 0//model.position
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
                 drag.minimumX: 0
                 drag.maximumX: background.width - knob.width
            }
        }
    }
    RangeModel {
        id: model
        minimumValue: 0
        maximumValue: 100
        minimumPosition: 0
        maximumPosition: background.width - knob.width
        position: knob.x
    }
}
