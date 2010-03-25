import Qt 4.6
import "../label"

Item {
    id:pushbutton
    property string text: "Text"
    signal clicked

    width:Math.max(text.width + 20, 200)
    height:Math.max(text.height + 12, 50)

    BorderImage {
        id: buttonImage
        source: "images/PushButton" + (parent.state=="Highlighted" ?
                                       "Highligted": "Normal") + ".png"
        anchors.fill:parent
        border.left:5;
        border.top:5;
        border.right:5;
        border.bottom:5;

    }
    BorderImage {
        id: buttonPressedImage
        opacity:0
        source: "images/PushButtonPressed.png"
        anchors.fill:parent
        border.left:5;
        border.top:5;
        border.right:5;
        border.bottom:5;
    }
    Label {
        id: text
        anchors.centerIn:parent
        text: pushbutton.text
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        onClicked: { pushbutton.clicked(); }
    }

    states: [
        State {
            name: "Highlighted"
            },
            State {
                name: "Disabled"
            },
            State {
                when: mouseRegion.pressed
                name: "Pressed"
                PropertyChanges { target: buttonPressedImage; opacity: "1"}
                PropertyChanges { target: text; color: "white"}
            }
    ]
    transitions: [
        Transition {
            from: "";
            to: "Pressed"
            NumberAnimation { properties: "opacity"; duration: 30 }
            ColorAnimation { properties: "color"; duration: 30 }
        },
        Transition {
            from: "Pressed";
            to: ""
            NumberAnimation { properties: "opacity"; duration: 150 }
            ColorAnimation { properties: "color"; duration: 150 }
        }
    ]

}
