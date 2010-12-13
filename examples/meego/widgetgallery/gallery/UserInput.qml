import Qt 4.7
import com.meego 1.0

Page {
    id: page
    anchors.fill: parent
    Rectangle {
        color: "steelblue"
        anchors.fill: parent

        LineEdit {
            id: lineEdit0
            width: 20
            text: "LineEdit"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            promptText:"lineEdit.promptText set ok"
            echoMode: TextInput.Password
        }

        LineEdit {
            id: lineEdit1
            width: 20
            text: "singleLineEdit"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: lineEdit0.bottom
            promptText:"lineEdit.promptText set ok"
        }

        MultiLineEdit {
            id: lineEdit2
            width: 20
            text: "MultiLineEdit"
            anchors.top: lineEdit1.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            promptText:"lineEdit.promptText set ok"
        }

        LineEdit {
            id: lineEdit3
            width: 20
            text: "vkb disabled"
            anchors.top: lineEdit2.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            promptText:"lineEdit.promptText set ok"
            //activateFocusOnPressed: false
        }

        TextInput {
            id: textInput
            color: "#ffffff"; selectionColor: "green"
            font.pixelSize: 42; font.bold: true
            anchors.top: lineEdit3.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "TextInput"
            MouseArea {
                anchors.fill: parent
                onReleased: {
                    textInput.focus=true;
                    screen.sendClicked(mouseX,mouseY,textInput.positionAt(mouseX));
                }
            }
        }
    }
}
