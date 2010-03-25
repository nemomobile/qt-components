import Qt 4.6

Item {
    id:lineedit
    property string label: "label"

    width: text.width + 11
    height: 13 + 11 

    BorderImage {
        source: "../mx-style/entry.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    BorderImage {
        id: focusframe
        opacity:0
        source: "../mx-style/entry-active.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    clip: true
    TextInput{
        id:text
        cursorDelegate: Item{
            Rectangle{
                visible: text.focus
                color: "#009BCE"
                height: 13
                width: 2
                y: 1
            }
        }
        anchors.centerIn: parent
        anchors.topMargin:5
        anchors.rightMargin:6
        anchors.bottomMargin:6
        anchors.leftMargin:5
        text:"Some random text"
        horizontalAlignment: TextEdit.AlignLeft
        font.pixelSize:13
        font.family: droidBold.name
        focusOnPress:true
    }
    states: [
        State {
            name: "Highlighted"
            when: text.focus
            PropertyChanges { target: focusframe; opacity: 1.0}
        },
        State { name: "Disabled" }
    ]
    transitions: [
        Transition {
            from: ""; to: "Highlighted"
            NumberAnimation { properties: "opacity"; duration: 150 }
        },
        Transition {
            from: "Highlighted"; to: ""
            NumberAnimation { properties: "opacity"; duration: 150 }
        }
    ]
}
