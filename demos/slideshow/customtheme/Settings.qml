import Qt 4.6

Item {
    id: settings;

    width: parent.width;
    height: parent.height;
    anchors.fill: parent;

    signal back;

    MouseArea {
        id: backArea;
        enabled: false;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: dialog.right;
        anchors.right: parent.right;
        onClicked: settings.back();
    }

    SettingsDialog {
        id: dialog;
        height: parent.height;
        width: parent.width * 0.6;
        anchors.bottom: parent.bottom;
    }

    states: [
        State {
            name: "hide";
            PropertyChanges {
                target: dialog;
                x: -parent.width;
            }
            PropertyChanges {
                target: backArea;
                enabled: false;
            }
        },
        State {
            name: "show";
            PropertyChanges {
                target: dialog;
            }
            PropertyChanges {
                target: backArea;
                enabled: true;
            }
        }
    ]

    transitions: [
        Transition {
            from: "hide";
            to: "show";
            NumberAnimation {
                properties: "x";
                easing.type: "OutCubic";
                duration: 800;
            }
        },
        Transition {
            from: "show";
            to: "hide";
            NumberAnimation {
                properties: "x";
                easing.type: "InCubic";
                duration: 800;
            }
        }
    ]
}
