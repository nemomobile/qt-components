import Qt 4.6


Item {
    id: checkbox
    width: 32; height: 32
    property bool enabled: true
    function dbg() {console.log('image.src: '+image.source+' / cb state: '+checkbox.state+' image state: '+image.state)}

    Image {
        id: image
        anchors.centerIn: parent
        source: "images/checkbox" + (state==""?"Normal": state) + "Unchecked.png"
        states: [
            State { name: "Disabled"; when: !checkbox.enabled },
            State { name: "Pressed" },
            State { name: "Highlighted" }
        ]
        Image {
            id: imagecheck
            opacity:0
            anchors.centerIn: parent
            source: "images/checkbox" + (image.state==""?"Normal": image.state) + "Checked.png"
        }
    }
    MouseArea {
        enabled: parent.enabled
        anchors.fill: parent
        property string checkboxState
        property url test
        // TODO: change to focus
        //hoverEnabled: true
        onEntered: { image.state = 'Highlighted'; }
        onExited: { image.state = '' }
        onPressed: { image.state = 'Pressed' }
        onReleased: {  checkbox.clicked() }
    }
    states: [
        State {
            name: "Checked"
            PropertyChanges {
                target: imagecheck
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
    function clicked() {
        image.state = '';
        if (checkbox.state == '') {
            checkbox.state = 'Checked'
        } else {
        checkbox.state = ''
        }
    }
}
