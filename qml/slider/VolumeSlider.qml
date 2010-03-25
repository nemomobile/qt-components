import Qt 4.6

Item {
    id: container;
    width:300
    height: 50
    property var sliderEdgeOffset: 6;

    Image {
        id: leftIcon
        source: "images/VolumeIndicatorLow" +
        (parent.state=="Highlighted" ? "Highlighted" : "") + ".png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left:parent.left
    }

    Image {
        id: rightIcon
        source: "images/VolumeIndicatorHigh.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
    }

    BasicSlider {
        anchors.left: leftIcon.right
        anchors.right : rightIcon.left
        anchors.top : parent.top
        anchors.bottom: parent.bottom
    }

}
