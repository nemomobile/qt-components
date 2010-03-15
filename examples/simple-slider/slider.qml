import Qt 4.6
import Components 1.0

Image {
    id: groove
    property alias value: model.value
    property int minimum: 25
    property int maximum: 75
    width: 200; height: 50
    fillMode: Image.Tile
    source: "stripes.png"
    BorderImage {
        id: grip
        x: 0; y: 0; width: 50; height: 50
        border.left: 10; border.top: 10
        border.right: 10; border.bottom: 10
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        source: "toolbutton.png"
        Text {
            id: text
            anchors.centerIn: parent
            text: model.value
            color: "white"
        }
        MouseRegion {
             anchors.fill: parent
             drag.target: grip
             drag.axis: "XAxis"
             drag.minimumX: 0
             drag.maximumX: groove.width - grip.width
        }
    }
    RangeModel {
        id: model
        minimumValue: groove.minimum
        maximumValue: groove.maximum
        minimumPosition: 0
        maximumPosition: groove.width - grip.width
        position: grip.x
    }
}
