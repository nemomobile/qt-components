import Qt 4.7

Item {
    Rectangle {
        anchors.fill: parent
        color: "tomato"
    }

    Rectangle {
        x: 10
        y: 10
        width: 10
        height: 10
        color: "ghostwhite"
    }

    Rectangle {
        x: 20
        y: 20
        width: 10
        height: 10
        color: mouseArea.pressed ? "firebrick" : "salmon"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}
