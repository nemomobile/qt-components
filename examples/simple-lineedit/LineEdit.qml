import Qt 4.6
import Components 1.0

FocusScope {
    property string emptyText: "Type some text..."
    property alias font: contents.font
    property alias text: contents.text

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

        x: contents.x + model.cursorX
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
            // ### we should be doing some calc and then setposition
            model.cursorX = mouse.x
        }
    }

    LineEditEventHelper {
        id: keyHandler
        focus: true
        model: model
        onAccepted: parent.accepted()
    }

    LineEditModel {
        id: model

        // ### TextLayout stuff
        font: contents.font
    }
}
