import Qt 4.6
import "../canolatheme"

Item {
    id: settingsView;

    x: 0;
    y: 0;

    width: parent.width;
    height: parent.height;
    anchors.fill: parent;

    signal back;

    Rectangle {
        id: background;
        color: "black";
        anchors.fill: parent;
    }

    MouseArea {
        id: backArea;
        enabled: false;
        anchors.top: parent.top;
        anchors.bottom: dialog.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        onClicked: settingsView.back();
    }

    Settings {
        id: dialog;
        height: parent.height * 0.8;
        anchors.left: parent.left;
        anchors.right: parent.right;
    }

    states: [
        State {
            name: "hide";
            PropertyChanges {
                target: background;
                opacity: 0;
            }
            PropertyChanges {
                target: dialog;
                y: parent.height;
            }
            PropertyChanges {
                target: backArea;
                enabled: false;
            }
        },
        State {
            name: "show";
            PropertyChanges {
                target: background;
                opacity: 0.7;
            }
            PropertyChanges {
                target: dialog;
                y: 0.2 * parent.height;
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
            from: "show";
            to: "hide";
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
            }
        }
    ]
}