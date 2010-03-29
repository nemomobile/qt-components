import Qt 4.7

Item {
    id: tooltip;
    property alias text: label.text;
    property bool shown: false;

    width: label.width + 22;
    height: label.height + 16;

    transformOrigin: Item.Top;
    scale: 0;
    visible: false;

    BorderImage {
        id: background;
        anchors.fill: parent;
        source: "images/tooltip-background.png";
        border.top: 4;
        border.left: 11;
        border.bottom: 12;
        border.right: 11;
    }

    Label {
        id: label;
        anchors.centerIn: parent;
        anchors.verticalCenterOffset: -4;
        color: "#ffffff";
    }

    states: [
        State {
            name: "shown";
            when: tooltip.shown && (label.text !== "");
            PropertyChanges { target: tooltip; scale: 1; visible: true }
        }
    ]

    transitions: [
        Transition {
            from: "";
            to: "shown";
            SequentialAnimation {
                PropertyAction {
                    target: tooltip;
                    property: "visible";
                }
                NumberAnimation {
                    duration: 500;
                    target: tooltip;
                    easing.type: "OutElastic";
                    easing.period: 0.25;
                    property: "scale";
                }
            }
        },
        Transition {
            from: "shown";
            to: "";
            SequentialAnimation {
                NumberAnimation {
                    duration: 150;
                    target: tooltip;
                    easing.type: "InSine";
                    property: "scale";
                }
                PropertyAction {
                    target: tooltip;
                    property: "visible";
                }
            }
        }
    ]
}
