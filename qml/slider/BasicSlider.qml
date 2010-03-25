import Qt 4.6
import "../pushbutton"

Item {
    id: basicSlider;
    property var sliderEdgeOffset: 6;
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    anchors.verticalCenter: parent.verticalCenter
    width:parent.width
    Rectangle {
        id: sliderBase
        color: '#dadada'
        width: parent.width
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
        PushButton {
            id: nob
            x: parent.width/2 - nob.width/2
            anchors.verticalCenter: parent.verticalCenter
            text:""
            width: 40
            height: 40
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
