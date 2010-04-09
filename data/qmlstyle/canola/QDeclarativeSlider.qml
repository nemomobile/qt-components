// canola theme slider

import Qt 4.6

Item {
    id: slider
    width: 300; height: 100
    BorderImage {
        id: background
        x: 0
        y: (parent.height - height) / 2
        width: parent.width
        border.left: 12;
        border.top: 12;
        border.right: 12;
        border.bottom: 12;
        source: "images/slide_background.png"
        Image {
            id: knob
            x: /*model.position*/ - (width / 2)
            y: (parent.height - height) / 2
            source: "images/slide_knob.png"
            MouseArea {
                 id: mouseArea
                 anchors.fill: parent
                 drag.target: knob
                 drag.axis: "XAxis"
                 drag.minimumX: -(knob.width / 2)
                 drag.maximumX: background.width - (knob.width / 2)
            }
        }
    }

    Binding { target: model; property: "minimumValue"; value: 0 }
    Binding { target: model; property: "maximumValue"; value: 100 }
    Binding { target: model; property: "positionAtMinimum"; value: 0 }
    Binding { target: model; property: "positionAtMaximum"; value: background.width - (knob.width / 2) }
    Binding { target: model; property: "position"; value: knob.x + (knob.width / 2) }
}
