import Qt 4.7
import com.meego 1.0

Rectangle {

    width: 500
    height: 400

    gradient: Gradient {
        GradientStop {position: 0; color: "lightgray" }
        GradientStop {position: 1; color: "#606060" }
    }

    // MouseArea {
    //     anchors.fill: parent
    //     onClicked: parent.focus = true
    // }

    Grid {
        columns: 2
        spacing: 10
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        Label {
            text: "Free text:"
            height: line1.height
            verticalAlignment: Text.AlignVCenter
        }

        LineEdit {
            id: line1
            width: parent.width - x
            promptText: "Enter text here"
        }

        Label {
            height: line2.height
            text: "Free text (masked):"
            verticalAlignment: Text.AlignVCenter
        }

        LineEdit {
            id: line2
            width: parent.width - x
            promptText: "This tooltip is quite long and will be clipped on screen"
            echoMode: TextInput.Password
        }

        Label {
            height: line3.height
            text: "No Echo mode:"
            verticalAlignment: Text.AlignVCenter
        }

        LineEdit {
            id: line3
            width: parent.width - x
            echoMode: TextInput.NoEcho
        }

        Label {
            height: line4.height
            text: "Echo on edit:"
            verticalAlignment: Text.AlignVCenter
        }

        LineEdit {
            id: line4
            width: parent.width - x
            echoMode: TextInput.PasswordEchoOnEdit
        }
    }
}