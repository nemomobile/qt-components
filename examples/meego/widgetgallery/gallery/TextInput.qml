import Qt 4.7
import com.meego 1.0

Page {
    id: page
    anchors.fill: parent
    tools: commonTools
    Rectangle {
        color: "steelblue"
        anchors.fill: parent
        TextField {
            id: lineEdit0
            width: 20
            text: "TextField"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            placeholderText: "lineEdit.promptText set ok"
            echoMode: TextInput.Password
        }

        TextField {
            id: lineEdit1
            width: 20
            text: "singleTextField"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: lineEdit0.bottom
            placeholderText: "lineEdit.promptText set ok"
        }

        TextArea {
            id: lineEdit2
            width: 20
            text: "TextArea"
            anchors.top: lineEdit1.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "lineEdit.promptText set ok"
        }

        TextField {
            id: lineEdit3
            width: 20
            text: "vkb disabled"
            anchors.top: lineEdit2.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "lineEdit.promptText set ok"
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
