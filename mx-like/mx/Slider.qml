import Qt 4.6

Item {
    id: basicSlider;
    property var sliderEdgeOffset: 6;
    property real value: nob.x/(sliderBase.width-nob.width)
    height: 22
    width: 108
    Rectangle {
        id: sliderBase
        color: '#dadada'
        width: parent.width - 12
        x:6
        anchors.verticalCenter: parent.verticalCenter
        height: 8
        Rectangle {
            id: sliderBar
            color: '#84dbf5'
            anchors.left: parent.left
            anchors.right : nob.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            height: 8
        }
        Button {
            id: nob
            x: parent.width/2 - nob.width/2
            anchors.verticalCenter: parent.verticalCenter
            text:""
            width: 22
            height: 16
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
