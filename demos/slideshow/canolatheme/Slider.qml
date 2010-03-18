import Qt 4.6
//import Components 1.0

Item {
    id: slider
    property alias value: model.value
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    width: 300; height: 100
    BorderImage {
        id: background
        x: 0
        y: (parent.height - 27) / 2
        width: parent.width
        border.left: 12;
        border.top: 12;
        border.right: 12;
        border.bottom: 12;
        source: "images/slide_background.png"
        Image {
            id: knob
            x: model.position;
            y: (parent.height - 83) / 2
            source: "images/slide_knob.png"
            MouseArea {
                 id: mouseArea
                 anchors.fill: parent
                 drag.target: knob
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
        sliderDown: mouseArea.pressed
    }
}
