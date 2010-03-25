// canola theme slider

import Qt 4.6
import Components 1.0

Item {
    id: slider
    property alias value: model.value
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    width: 300; height: 100
    BorderImage {
        id: background
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

    RangeModel {
        id: model
        maximumPosition: background.width - knob.width
        position: knob.x + (knob.width / 2)
    }
}
