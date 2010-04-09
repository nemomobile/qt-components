// dui theme slider

import Qt 4.6

Item {
    id: slider
    width: 300; height: 100
    BorderImage {
        id: background
        x: 0
        y: (parent.height - height) / 2
        width: parent.width
        border.left: 6;
        border.right: 6;
        source: "images/slider-background.svg"
        BorderImage {
            id: elapsed
            source: "images/slider-elapsed.svg"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: knob.x + (knob.width / 2);
            border.left: 6;
            border.right: 6;
        }
        Item {
            id: knob
            x: /*model.position*/ - (knob_normal.width / 2);
            y: (parent.height - knob_normal.height) / 2
            width: knob_normal.width
            height: knob_normal.height
            Image {
                id: knob_normal
                source: "images/slider-knob.svg"
                visible: !mouseArea.pressed;
            }
            Image {
                id: knob_pressed
                source: "images/slider-knob-pressed.svg"
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

    Binding { target: model; property: "minimumValue"; value: 0 }
    Binding { target: model; property: "maximumValue"; value: 100 }
    Binding { target: model; property: "positionAtMinimum"; value: 0 }
    Binding { target: model; property: "positionAtMaximum"; value: background.width - knob.width }
    Binding { target: model; property: "position"; value: knob.x + (knob.width / 2) }
}
