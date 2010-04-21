import Qt 4.7
import Components 1.0

FocusScope {
    id: component

    property string emptyText: "Type some text..."
    property alias font: contents.font
    property alias text: model.text
    property bool password: false

    signal accepted

    width: contents.width + 20
    height: contents.height + contents.y + 50

    Text {
        id: contents
        font.family: "Liberation Sans"
        font.pointSize: 14

        y: 20

        color: model.displayText ? "black" : "gray"
        text: keyHandler.focus || model.displayText ? model.displayText : emptyText
    }

    Rectangle {
        id: cursor
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 1

        color: keyHandler.focus ? "tomato" : "gray"

        // ###
        x: contents.x + layout.cursorX
        Behavior on x { NumberAnimation { duration: 100 } }

        visible: keyHandler.focus || model.displayText

        Text {
            font.family: "Helvetica"
            font.pointSize: 8
            anchors.left: cursor.right
            anchors.leftMargin: 4
            anchors.bottom: cursor.bottom
            text: model.cursorPosition
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.focus = true
            model.cursorPosition = layout.xToPosition(mouse.x)
            print(layout.cursorX)
        }
    }

    LineEditEventHelper {
        id: keyHandler
        model: model
        focus: true
        onAccepted: parent.accepted()
    }

    LineEditLayoutHelper {
        id: layout
        model: model
        font: contents.font
    }

    LineEditModel {
        id: model
        // This simulates PasswordEchoOnEdit
        echoMode: (!component.password || keyHandler.focus) ? LineEditModel.Normal : LineEditModel.Password
    }
}
