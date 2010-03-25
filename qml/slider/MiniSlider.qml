import Qt 4.6

Item {
    id: basicSlider;
    width:200
    height: 50
    property var sliderEdgeOffset: 6;
    anchors.leftMargin: 10
    anchors.rightMargin: 10

    Rectangle{
        id: sliderBase
        width:parent.width-2
        anchors.verticalCenter: parent.verticalCenter
        color:"#808080"
        height: 3
        Image {
            id: nob
            source: "images/SliderThumb" + (mouseRegion.pressed ? "Pressed" :
            parent.state=="Highlighted" ? "Highlighted" : "Normal") + ".png"
            x:parent.width/2 - nob.width/2
            anchors.verticalCenter: parent.verticalCenter
        }
        Bubble {
            id: bubble
            anchors.bottom:nob.top
            anchors.horizontalCenter: nob.horizontalCenter
            anchors.bottomMargin:7
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
    states: [
        State {
            name: "Highlighted"
        },
        State {
            when: mouseRegion.pressed
            name: "Pressed"
            PropertyChanges { target: bubble; state:"Pressed" }
        }
    ]
}
