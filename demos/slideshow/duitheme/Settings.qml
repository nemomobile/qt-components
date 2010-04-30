import Qt 4.7

Item {
    id: settings;

    property alias interval: dialog.interval;
    property string animatorName: "fade";

    Rectangle {
        id: background;
        color: "black";
        anchors.fill: parent;
        opacity: 0;

        /* effects are not in 4.7, have to expose the C++ class to qml
        effect: Blur {
            id: blureffect;
            blurRadius: 10;
        }*/
    }

    MouseArea {
        id: backArea;
        enabled: false;
        anchors.top: dialog.bottom;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        onClicked: { settings.state = ""; }
    }

    SettingsDialog {
        id: dialog;
        height: parent.height * 0.6;
        anchors.left: parent.left;
        anchors.right: parent.right;
        y: -parent.height;
    }

    states: [
        State {
            name: "shown";
            PropertyChanges {
                target: background;
                opacity: 0.7;
            }
            PropertyChanges {
                target: dialog;
                y: 0;
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
            SequentialAnimation {
                NumberAnimation {
                    properties: "opacity";
                    easing.type: "OutCubic";
                    duration: 100;
                }
                NumberAnimation {
                    properties: "y";
                    easing.type: "OutCubic";
                    duration: 800;
                }
            }
        },
        Transition {
            from: "shown";
            to: "";
            SequentialAnimation {
                NumberAnimation {
                    properties: "y";
                    easing.type: "OutCubic";
                    duration: 800;
                }
                NumberAnimation {
                    properties: "opacity";
                    easing.type: "OutCubic";
                    duration: 100;
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
