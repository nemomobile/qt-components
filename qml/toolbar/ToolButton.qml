import Qt 4.6


Item {
    width: background.width
    height: background.height
    id: toolbutton
    property string icon: "images/ToolButton_default.png"
    property bool enabled: true
    signal clicked

    Image {
        x: width / 2
        source: "images/ToolBar_separator.png"
        visible: toolbutton.parent.children[0] != toolbutton
    }

    Image {
        id: background
        opacity:0
        source: "images/ToolBar_pressed.png"
    }
    Image {
        id: decoration
        source: toolbutton.icon
        anchors.centerIn: toolbutton
    }
    MouseArea {
        id: mouseRegion
        enabled: toolbutton.enabled
        anchors.fill: parent
        onClicked: toolbutton.clicked()
    }
    states: [
        State {
            name: "Disabled"
            when: !toolbutton.enabled || !toolbutton.parent.parent.enabled
            PropertyChanges { target:background; opacity: 1; source: "images/ToolBar_Disabled.png" }
        },
        State {
            name: "Pressed"
            when: mouseRegion.pressed
            PropertyChanges { target:background; opacity: 1}
        },
        State {
            name: "Highlighted"
            when: toolbutton.focus
            PropertyChanges { target:background; opacity: 1; source: "images/ToolBar_highlighted.png" }
        }
    ]
    transitions: [
        Transition {
            from: ""; to: "Pressed"
            NumberAnimation { properties: "opacity"; duration: 30 }
        },
        Transition {
            from: "Pressed"; to: ""
            NumberAnimation { properties: "opacity"; duration: 100 }
        }
    ]
}
