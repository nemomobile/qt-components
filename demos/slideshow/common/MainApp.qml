import Qt 4.6

Item {
    id: mainApp;
    width: 800;
    height: 480;

    property alias viewer: viewer;
    property alias playpausebutton: playpausebutton;
    property alias settingsbutton: settingsbutton;

    Viewer {
        id: viewer;
        interval: 2000;
        running: false;
        Component.onCompleted: {
            path = "photos/";
            state = "fade";
        }
    }

    PlayPauseButton {
        id: playpausebutton;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        onClicked: {
            viewer.running = !viewer.running;
        }
    }

    SettingsButton {
        id: settingsbutton;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        onClicked: {
            viewer.running = false;
            settingsDialog.state = "show";
        }
    }
}
