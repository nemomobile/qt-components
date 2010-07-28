import Qt 4.7
import Qt.labs.components 1.0
import themebridge 1.0

Rectangle {
    color: "darkgray"
    width: 640
    height: 480

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

        Button { onClicked: icon.iconId = "icon-m-telephony-call-answer"}
    }

    Icon {
        id: icon
        anchors.centerIn: parent

        style: meegostyle
        iconId: "icon-m-framework-home"

        Style {
            id: meegostyle
            styleType: Style.Button
        }

        // Rectangle {
        //     color: "green"
        //     opacity: 0.15
        //     anchors.fill: parent
        // }
    }
}