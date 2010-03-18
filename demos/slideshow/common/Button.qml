import Qt 4.6

Rectangle {
    id: root
    width: 40
    height: 40

    property string url
    signal clicked

    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked
    }
}
