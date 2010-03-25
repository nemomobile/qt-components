import Qt 4.6

Item {
    id: basicSlider;
    property var sliderEdgeOffset: 6;
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    anchors.verticalCenter: parent.verticalCenter
    width:parent.width
    BorderImage {
        id: sliderBase
        source: "images/SliderBase.png"
        width:parent.width
        anchors.verticalCenter: parent.verticalCenter
        border.left: 5
        border.right: 5
        border.top: 1
        border.bottom: 1
        height: 7

        BorderImage {
            id: sliderBar
            source: "images/SliderGreenBar.png"
            anchors.left: parent.left
            anchors.right : nob.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            border.left: 5
            border.right: 5
            border.top: 1
            border.bottom: 1
        }
        Image {
            id: nob
            source: "images/SliderThumb" + (mouseRegion.pressed ? "Pressed" :
            parent.state=="Highlighted" ? "Highlighted" : "Normal") + ".png"
            x:parent.width/2 - nob.width/2
            anchors.verticalCenter: parent.verticalCenter
        }
        Bubble {
            id: bubble
            state:mouseRegion.pressed ? "Pressed" : ""
            anchors.bottom:nob.top
            anchors.bottomMargin:7
            anchors.horizontalCenter: nob.horizontalCenter
            label:Math.floor(100 * ((nob.x+nob.width/2 - sliderEdgeOffset)/(sliderBase.width - 2*sliderEdgeOffset)))
        }
        MouseArea {
            function handleRelease(x) {nob.x = x}
            anchors.verticalCenter:parent.verticalCenter
            anchors.left:parent.left;
            anchors.right:parent.right;
            height:20
            onPressed: { handleRelease(mouseX);}
        }
        MouseArea {
            id:mouseRegion
            anchors.verticalCenter:nob.verticalCenter
            anchors.horizontalCenter:nob.horizontalCenter
            width:50
            height:50
            drag.target: nob
            drag.axis: "XAxis"
            drag.minimumX:-sliderEdgeOffset
            drag.maximumX:sliderBase.width-nob.width/2 - sliderEdgeOffset
        }
    }
}
