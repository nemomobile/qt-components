import Qt 4.6

Item {
    id: mainApp
    width: 640
    height: 480

    //###
    property alias view: viewer.source

    Viewer {
        id: viewer
    }

    Panel {
        id: panel

        Button {
            id: playButton
            color: "red"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        Button {
            id: pauseButton
            color: "blue"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: settingsButton
            color: "green"

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
        }
    }

}
