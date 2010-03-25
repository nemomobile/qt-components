import Qt 4.6

Rectangle {
    id: radioButton
    property string text: "label"
    property bool selected: (parent.selectedButton==radioButton)

    Image {
        id: radioButtonImage
        source: "images/RadioButton"
            + (radioButton.focus?"Highlighted":"Normal") + ".png"
            Image {
                id: radioButtonCheck
                opacity:0
                source: "images/RadioButton"
                    + (radioButton.focus?"Highlighted":"Normal")
                    + "Selected.png"
            }
        MouseArea {
            id: radioButtonRegion
            anchors.fill: parent
            onClicked: { radioButton.focus = true; radioButton.parent.selectedButton=radioButton }
        }
        states: [
            State {
                when: (selected==true);
                name: "Checked"
                PropertyChanges {
                    target: radioButtonCheck
                    opacity: 1.0
                }
            }
        ]
        transitions: [
            Transition {
                from: ""; to: "Checked"
                NumberAnimation { properties: "opacity"; duration: 150 }
            },
            Transition {
                from: "Checked"; to: ""
                NumberAnimation { properties: "opacity"; duration: 150 }
            }
        ]
    }
    Text {
        anchors.left: radioButtonImage.right
        anchors.verticalCenter: radioButtonImage.verticalCenter
        anchors.right: parent.right
        anchors.leftMargin: 5
        text: radioButton.text
    }

    states: [
        State {
            name: "Highlighted"
            when: radioButton.focus && !radioButtonRegion.pressed
        },
        State {
            name: "Disabled"
            when: radioButton.disabled
            PropertyChanges {
                target: radioButtonImage
                source: "images/RadioButtonDisabled"+ (selected?"Selected":"") +".png"
            }
        },
        State {
            name: "Pressed"
            when: radioButtonRegion.pressed
            PropertyChanges {
                target: radioButtonImage
                source: "images/RadioButtonPressed.png"
            }
        }
    ]
}
