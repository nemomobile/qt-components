import Qt 4.7
import Qt.labs.components 1.0

Rectangle {
    color: "darkgray"

    Column {
        anchors.fill: parent

        ButtonGroup {
            id: group

            Button {}
            Button {}
            Button {}
        }

        ButtonGroup {
            Button {}
        }

        Button { }
    }
}