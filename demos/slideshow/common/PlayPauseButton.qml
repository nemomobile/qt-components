import Qt 4.7

Item {
    id: button;
    width: 91;
    height: 88;

    property bool playing: false;
    signal clicked;

    Image {
        id: background;
        anchors.fill: parent;
        source: "images/button_background.png";
    }

    Image {
        id: playIcon;
        anchors.centerIn: parent;
        visible: !button.playing;
        source: "images/icon_play.png";
    }

    Image {
        id: pauseIcon;
        anchors.centerIn: parent;
        visible: button.playing;
        source: "images/icon_pause.png";
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
        onClicked: {
            button.playing = !button.playing
            button.clicked();
        }
    }
}
