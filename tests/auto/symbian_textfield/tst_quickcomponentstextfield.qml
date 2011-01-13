import Qt 4.7
import com.nokia.symbian 1.0

Column {
    id: column
    anchors.leftMargin: 5
    anchors.topMargin: 5
    anchors.rightMargin: 5
    anchors.bottomMargin: 5
    spacing: 5
    height: screen.height
    width: screen.width

    Component.onCompleted: screen.orientation = Screen.Portrait

    TextField {
        id: textField
        objectName: "textField"
        text: "test"
        width: parent.width
    }

    TextField {
        id: textFieldWithValidator
        objectName: "textFieldWithValidator"
        text: "50"
        width: parent.width
        validator: IntValidator {bottom: 11; top: 31;}
    }

    TextField {
        id: textFieldImpSize
        objectName: "textFieldImpSize"
    }
}
