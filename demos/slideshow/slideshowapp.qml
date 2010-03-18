import Qt 4.6
import "common"

Item {
    id: mainApp;
    width: 800;
    height: 480;

    //###
    property alias view: viewer.source

    Viewer {
        id: viewer;
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
    }
}
