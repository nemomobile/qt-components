import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Rectangle {
    color: "darkgray"
    width: 640
    height: 480

    Column {
        anchors.fill: parent

        ButtonGroup {
            id: group

            Button {width: 100; height: 50}
            Button {width: 100; height: 50}
            Button {width: 100; height: 50}
        }

        ButtonGroup {
            Button {width: 100; height: 50}
        }

        Button { width: 200; height: 100; onClicked: icon.iconId = "icon-m-telephony-call-answer"}
    }

    Icon {
        id: icon
        anchors.centerIn: parent
        iconId: "icon-m-framework-home"

        // Rectangle {
        //     color: "green"
        //     opacity: 0.15
        //     anchors.fill: parent
        // }
    }
}
