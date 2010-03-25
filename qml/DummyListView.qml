import Qt 4.6

ListView {
    id: flickable
    delegate: Component {
        Rectangle {
            border.color: "grey";
            width: parent.width-11; height: 60;
            Text {
                text: "Lorem ipsum"
                width: parent.width
                anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right
                anchors.topMargin: 5; anchors.leftMargin: 5; anchors.rightMargin: 5
                font.family: droidSans.name
                font.pointSize: 18
                font.bold: true
            }
            Text {
                text: "index " + index
                anchors.top: parent.top; anchors.right: parent.right;
                anchors.topMargin: 5; anchors.leftMargin: 5; anchors.rightMargin: 5
                font.family: droidSans.name
                font.pointSize: 12
                font.bold: true
            }
            Text {
                text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit";
                width: parent.width
                anchors.bottom: parent.bottom; anchors.left: parent.left; anchors.right: parent.right
                anchors.bottomMargin: 5; anchors.leftMargin: 5; anchors.rightMargin: 5
                font.family: droidSans.name
                font.pointSize: 18
            }
            Rectangle { width: parent.width; height: 0.5; anchors.bottom:parent.bottom }
        }
    }
    model:30
    clip: true
}
