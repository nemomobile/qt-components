import Qt 4.6
import "common"

Item {
    id: mainApp;
    width: 800;
    height: 480;

    Viewer {
        id: viewer;
        interval: 2000;
        running: false;
        Component.onCompleted: animator = "common/FadeAnimator.qml";
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

    SettingsView {
        id: settingsDialog;
        state: "hide";
        onBack: {
            viewer.running = true;
            settingsDialog.state = "hide";
        }
    }
}
