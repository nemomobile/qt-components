import Qt 4.7
import com.meego 1.0

Page {
    id: page
    anchors.fill: parent
    tools: commonTools

    TextField {
        id: lineEdit0
        width: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        placeholderText: "Password field"
        echoMode: TextInput.Password
    }

    TextField {
        id: lineEdit1
        width: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: lineEdit0.bottom
        placeholderText: "Single-line text field"
    }

    TextArea {
        id: lineEdit2
        width: 20
        anchors.top: lineEdit1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        placeholderText: "Multi-line text area"
    }

    TextInput {
        id: textInput
        selectionColor: "green"
        font.pixelSize: 42; font.bold: true
        anchors.top: lineEdit2.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: "Inline text input"
        MouseArea {
            anchors.fill: parent
            onReleased: {
                textInput.focus=true;
                screen.sendClicked(mouseX,mouseY,textInput.positionAt(mouseX));
            }
        }
    }
}
