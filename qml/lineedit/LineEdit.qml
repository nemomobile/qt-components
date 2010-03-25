import Qt 4.6

Item {
    id:lineedit
    property string label: "label"

    width: Math.min(260, Math.max(text.width + 24, 260))
    height: Math.max(text.height + 18, 50)

    BorderImage {
        source: "images/LineEditNormal.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    BorderImage {
        id: focusframe
        opacity:0
        source: "images/LineEditHighlighted.png"
        anchors.fill:parent
        border.left:5; border.top:5;
        border.right:5; border.bottom:5;
    }
    clip: true
    TextEdit{
        id:text
        anchors.fill:parent
        anchors.left:parent.left
        anchors.leftMargin:15
        anchors.topMargin:15
        anchors.bottomMargin:15
        anchors.rightMargin:15
        //wrap: true
        text:"Some random text"
        horizontalAlignment: TextEdit.AlignLeft
        font.pointSize:12
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
