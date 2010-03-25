import Qt 4.6

// Reusable component for blue value-bubble

BorderImage {
    id: bubble
    opacity:0
    property alias label: label.text
    source: "images/Bubble.png"
    width:Math.max(34, label.width + 16)
    border.left:6;
    border.top:6;
    border.right:6;
    border.bottom:6;
    height:27
    Image {
        id: arrow
        source: "images/Arrow.png"
        anchors.top:bubble.bottom;
        anchors.topMargin:-5
        anchors.horizontalCenter:bubble.horizontalCenter
    }
    Text {
        id:label
        anchors.verticalCenter:parent.verticalCenter
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.verticalCenterOffset: -1
        anchors.horizontalCenterOffset: -1
        color:"white"
    }
    transitions: [
        Transition {
            from: ""; to: "Pressed"
            NumberAnimation { properties: "opacity"; duration: 140 }
        },
        Transition {
            from: "Pressed"; to: ""
            NumberAnimation { properties: "opacity"; duration: 250 }
        }
    ]

    states: [
        State {
            name: "Pressed"
            PropertyChanges { target: bubble; opacity: 1 }
        },
        State {
            name: ""
            PropertyChanges { target: bubble; opacity: 0 }
        }
    ]
}
