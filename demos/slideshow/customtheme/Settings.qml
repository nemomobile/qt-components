import Qt 4.7

Item {
    id: settings;

    property alias interval: dialog.interval;
    property string animatorName: "fade";

    MouseArea {
        id: backArea;
        enabled: false;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: dialog.right;
        anchors.right: parent.right;
        onClicked: { settings.state = ""; }
    }

    SettingsDialog {
        id: dialog;
        height: parent.height;
        width: parent.width * 0.6;
        anchors.bottom: parent.bottom;
        x: -parent.width;
    }

    states: [
        State {
            name: "shown";
            PropertyChanges {
                target: dialog;
                x: 0;
            }
            PropertyChanges {
                target: backArea;
                enabled: true;
            }
        }
    ]

    transitions: [
        Transition {
            from: "";
            to: "shown";
            NumberAnimation {
                properties: "x";
                easing.type: "OutCubic";
                duration: 800;
            }
        },
        Transition {
            from: "shown";
            to: "";
            SequentialAnimation {
                NumberAnimation {
                    properties: "x";
                    easing.type: "InCubic";
                    duration: 800;
                }
                ScriptAction {
                    // This delays the actual change of the animator to after
                    // the animation end. Avoids multiple loading and unloading
                    // of animators in the button click handler.
                    script: { animatorName = dialog.animatorName; }
                }
            }
        }
    ]
}
