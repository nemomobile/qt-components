import Qt 4.7

Item {
    id: pushbutton

    property string text: "Text"
    property alias tooltipText: tooltip.text;
    property bool checkable: false
    property bool checked: false
    signal clicked

    width: Math.max(text.width + 20, 200)
    height: Math.max(text.height + 12, 50)

    BorderImage {
        id: buttonImage
        source: "images/button.png"
        anchors.fill:parent
        border.left:10;
        border.top:10;
        border.right:10;
        border.bottom:10;
    }

    BorderImage {
        id: buttonActiveImage
        source: "images/button-active.png"
        opacity: 0

        anchors.fill: parent
        border.left: 10
        border.top: 10
        border.right: 10
        border.bottom: 10
    }

    BorderImage {
        id: buttonHoveredImage
        source: "images/button-hover.png"
        opacity: 0

        anchors.fill: parent
        border.left: 10
        border.top: 10
        border.right: 10
        border.bottom: 10
    }

    Text {
        id: text
        anchors.verticalCenter:parent.verticalCenter
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.verticalCenterOffset: pushbutton.state === "pressed" ? 1 : 0
        anchors.horizontalCenterOffset: pushbutton.state === "pressed" ? 1 : 0
        text: pushbutton.text
    }

    // ### I wish to have that as a separate item
    Tooltip {
        id: tooltip;
        anchors.top: pushbutton.bottom;
        anchors.horizontalCenter: pushbutton.horizontalCenter;

        property bool pressDismiss: false;
        shown: (mouseRegion.containsMouse && !pressDismiss);
    }

    MouseArea {
        id: mouseRegion
        hoverEnabled: true
        anchors.fill: parent
        onPressed: { tooltip.pressDismiss = true; }
        onExited: { tooltip.pressDismiss = false; }
        onClicked: {
            if (pushbutton.checkable) {
                pushbutton.checked = !pushbutton.checked;
            }
            pushbutton.clicked();
        }
    }

    states: [
        State {
            name: "pressed"
            when: mouseRegion.pressed || pushbutton.checked
            PropertyChanges { target: buttonImage; opacity: 0 }
            PropertyChanges { target: buttonActiveImage; opacity: 1 }
            PropertyChanges { target: buttonHoveredImage; opacity: 0 }
        },
        State {
            name: "highlighted"
            when: mouseRegion.containsMouse && !pushbutton.checked
            PropertyChanges { target: buttonImage; opacity: 0 }
            PropertyChanges { target: buttonActiveImage; opacity: 0 }
            PropertyChanges { target: buttonHoveredImage; opacity: 1 }
        }
    ]

    transitions: [
        /*Transition {
            from: "";
            to: "Pressed"
            NumberAnimation { properties: "opacity"; duration: 30 }
            ColorAnimation { properties: "color"; duration: 30 }
        },*/
        Transition {
            from: "highlighted";
            to: ""
            NumberAnimation { properties: "opacity"; duration: 130 }
        }
    ]

}
