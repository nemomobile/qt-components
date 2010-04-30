import Qt 4.7

Item {
    width: 800;
    height: 480;

    // Settings delegate component is expected to have:
    // - A "shown" state
    // - interval property: to indicate the interval between changes
    // - animatorName property: to indicate which animator should be used
    property alias settingsDelegate: settingsLoader.sourceComponent;
    property alias settings: settingsLoader.item;

    Viewer {
        id: viewer;
        interval: settings.interval;
        running: playpausebutton.playing && (settings.state !== "shown");
        path: "photos/"
        state: settings.animatorName;
    }

    PlayPauseButton {
        id: playpausebutton;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
    }

    SettingsButton {
        id: settingsbutton;
        anchors.right: parent.right;
        anchors.bottom: parent.bottom;
        onClicked: { settings.state = "shown"; }
    }

    Loader {
        id: settingsLoader
        anchors.fill: parent

        onStatusChanged: {
            if (status == Loader.Ready) {
                settings.anchors.fill = settingsLoader;
            }
        }
    }
}
