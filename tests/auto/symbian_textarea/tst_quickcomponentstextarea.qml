import Qt 4.7
import com.nokia.symbian 1.0

Column {
    id: testItem
    objectName: "testItem"

    anchors.fill: parent
    anchors.leftMargin: 5
    anchors.topMargin: 5
    anchors.rightMargin: 5
    anchors.bottomMargin: 5
    width: 360
    height: 640
    spacing: 5

    TextArea {
        id: textArea
        objectName: "testTextArea"
        width: parent.width
        height: Math.floor(parent.height * 0.9)
    }
    Button {
        id: testButton
        objectName: "testButton"
        text: "Click Me!"
        width: parent.width
        height: Math.floor(parent.height * 0.1)
        onClicked: {
            forceActiveFocus()
        }
    }
}
