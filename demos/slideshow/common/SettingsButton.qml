import Qt 4.7

Item {
    id: button;
    width: 91;
    height: 88;

    signal clicked;

    Image {
        id: background;
        anchors.fill: parent;
        source: "images/button_background.png";
    }

    Image {
        id: icon;
        anchors.centerIn: parent;
        source: "images/icon_settings.png";
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
        onClicked: button.clicked();
    }
}
